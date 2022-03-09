/******************************************************************************/
/*!
\file		Map.cpp
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Map.h"
#include "MapFactory.h"
#include "glm/glm.hpp"

#include "SystemGraphics.h"

constexpr auto STRING_MAPTILE_0 = "Resources/Sprites/Tile0_test.png";
constexpr auto STRING_MAPTILE_1 = "Resources/Sprites/Tile1_test.png";

namespace FwEngine
{

	Map::Map() : _mapArray{}, _height(0), _width(0), _tileTex(), _noColIndex(10)
	{
	}

	Map::~Map()
	{
		/*
		if (_mapArray)
			delete[] _mapArray;
			*/
	}

	void Map::Init(std::string fileName)
	{
		MapFactory mapFactory;
		std::vector<std::string> textures;
		mapFactory.BuildMap(fileName, textures, this);
		InitTex(textures);

	}

	void Map::InitTex(std::vector<std::string> textures)
	{
		_tileTex.clear();

		for (std::string i : textures)
		{
			try
			{
				_tileTex.push_back(RESOURCE->GetTexture(i));
			}
			catch (...)
			{
				_tileTex.push_back(0);
				continue;
			}
		}
		/*
		_tileTex[0] = RESOURCE->GetTexture(STRING_MAPTILE_0);
		_tileTex[1] = RESOURCE->GetTexture(STRING_MAPTILE_1);
		*/
	}

	void Map::Free()
	{
		_mapArray.clear();
		/*
		if (_mapArray)
			delete[] _mapArray;

		_mapArray = nullptr;
		*/

		_height = 0;
		_width = 0;

		_tileTex.clear();
	}

	int Map::GetCell(int inWidth, int inHeight)
	{
		if (inWidth < 0 || inHeight < 0 || inWidth >= _width || inHeight >= _height)
			//throw "GG";
			return 0;
		return _mapArray[(size_t)inHeight * _width + inWidth];
	}

	int Map::GetCellCol(int inWidth, int inHeight)
	{
		if (inWidth < 0 || inHeight < 0 || inWidth >= _width || inHeight >= _height)
			return 0;
		return (_mapArrayCol[(size_t)inHeight * _width + inWidth] == 0 ? 0 : _noColIndex);
	}

	int Map::CheckCellCollision(
		float PosX, float PosY, 
		float xLeft, float xRight,
		float yBot, float yTop
	)
	{
		int flag = 0;

		PosX *= MAP_SCALE_INVERSE;
		PosY *= MAP_SCALE_INVERSE;

		xLeft *= MAP_SCALE_INVERSE;
		xRight *= MAP_SCALE_INVERSE;
		yBot *= MAP_SCALE_INVERSE;
		yTop *= MAP_SCALE_INVERSE;

		//PosX += 1.0f;
		//PosY += 1.0f;

		auto checkHotspot = [&flag, this](float hotspotX, float hotspotY, int collision_flag)
		{
			if (GetCellCol(static_cast<int>(hotspotX), static_cast<int>(hotspotY)) >= _noColIndex)
			{
				flag = flag | collision_flag;
			}
		};

		/* top */
		checkHotspot(PosX + xRight * 0.5f + 0.5f, PosY + yTop + 0.5f, COLLISION_TOP);
		checkHotspot(PosX - xLeft * 0.5f + 0.5f, PosY + yTop + 0.5f, COLLISION_TOP);
		/* bottom */
		checkHotspot(PosX + xRight * 0.5f + 0.5f, PosY - yBot + 0.5f, COLLISION_BOTTOM);
		checkHotspot(PosX - xLeft * 0.5f + 0.5f, PosY - yBot + 0.5f, COLLISION_BOTTOM);
		/* left */
		checkHotspot(PosX - xLeft + 0.5f, PosY - yBot * 0.5f + 0.5f, COLLISION_LEFT);
		checkHotspot(PosX - xLeft + 0.5f, PosY + yTop * 0.5f + 0.5f, COLLISION_LEFT);
		/* right */
		checkHotspot(PosX + xRight + 0.5f, PosY - yBot * 0.5f + 0.5f, COLLISION_RIGHT);
		checkHotspot(PosX + xRight + 0.5f, PosY + yTop * 0.5f + 0.5f, COLLISION_RIGHT);

		return flag;

	}

	float Map::SnapToCell(float current, float scale, int flag)
	{
		//UNREFERENCED_PARAMETER(Coordinate);
		//int coord = static_cast<int>(*Coordinate);
		//*Coordinate = static_cast<float>(coord) + 0.5f;

		//if (flag & COLLISION_BOTTOM || flag & COLLISION_LEFT)
		//{
		//	int current_i = static_cast<int>((current * MAP_SCALE_INVERSE + 0.5f));
		//	return ((float)current_i - 0.5f) * MAP_SCALE + scale + MAP_SCALE_P;
		//}
		//if (flag & COLLISION_TOP || flag & COLLISION_RIGHT)
		//{
		//	int current_i = static_cast<int>((current * MAP_SCALE_INVERSE + 0.5f));
		//	return ((float)current_i + 0.5f) * MAP_SCALE - scale - MAP_SCALE_P;
		//}

		if (flag & COLLISION_BOTTOM)
		{
			int current_i = static_cast<int>(((current - scale + MAP_SCALE_HF)* MAP_SCALE_INVERSE + 0.5f));
			return ((float)current_i - 0.5f) * MAP_SCALE + scale + MAP_SCALE_P;
		}
		if (flag & COLLISION_LEFT)
		{
			int current_i = static_cast<int>(((current - scale + MAP_SCALE_HF)* MAP_SCALE_INVERSE + 0.5f));
			return ((float)current_i - 0.5f) * MAP_SCALE + scale + MAP_SCALE_P;
		}
		if (flag & COLLISION_TOP)
		{
			int current_i = static_cast<int>(((current + scale - MAP_SCALE_HF)* MAP_SCALE_INVERSE + 0.5f));
			return ((float)current_i + 0.5f) * MAP_SCALE - scale - MAP_SCALE_P;
		}
		if (flag & COLLISION_RIGHT)
		{
			int current_i = static_cast<int>(((current + scale - MAP_SCALE_HF) * MAP_SCALE_INVERSE + 0.5f));
			return ((float)current_i + 0.5f) * MAP_SCALE - scale - MAP_SCALE_P;
		}

		return 0;
	}


	void Map::Draw(float topBound, float botBound, float leftBound, float rightBound)
	{
		if (!_mapArray.size())
			return;

		RESOURCE->GetShader()->ChangeShader(TEXTURE);
		RESOURCE->GetShader()->UseShader();
		glm::mat4 mvp(1);
		glm::mat4 model(1);


		glm::mat4 rotateMat(1), scaleMat(1);
		scaleMat = glm::scale(scaleMat, (glm::vec3)Vector3D(MAP_SCALE, MAP_SCALE, 1));
		//rotateMat = glm::rotate(rotateMat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		int count = 0;

		int startHeight = static_cast<int>(botBound * MAP_SCALE_INVERSE);
		if (startHeight < 0)
			startHeight = 0;

		int endHeight = static_cast<int>(topBound * MAP_SCALE_INVERSE) + 2;
		if (endHeight > _height)
			endHeight = _height;

		int startWidth = static_cast<int>(leftBound * MAP_SCALE_INVERSE);
		if (startWidth < 0)
			startWidth = 0;

		int endWidth = static_cast<int>(rightBound * MAP_SCALE_INVERSE) + 2;
		if (endWidth > _width)
			endWidth = _width;

		for (int h = startHeight; h < endHeight; ++h)
		{
			for (int w = startWidth; w < endWidth; ++w)
			{
				size_t index = _mapArray[(size_t)h * _width + w];
				if (index)
				{
					glm::mat4 translateMat(1);
					translateMat = glm::translate(translateMat, glm::vec3((float)w * MAP_SCALE, (float)h * MAP_SCALE, 1.0f));
					model = translateMat /** rotateMat*/ * scaleMat;

					mvp = GRAPHICS->_projection * GRAPHICS->_view * model;

					RESOURCE->GetShader()->SetUniformMat4("MVP", mvp);

					//float randomValueRed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					//float randomValueGreen = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					//float randomValueBlue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					//float randomValueAlpha = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
					RESOURCE->GetShader()->SetUniformVec4("Colour", FwMath::Vector4D{ 1.0f,1.0f,1.0f,1.0f });

					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glBindVertexArray(RESOURCE->GetMesh(SPRITE)._vao);
					//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx->_EBO);
					//glBindBuffer(GL_ARRAY_BUFFER, gfx->_VBO);

					glBindTexture(GL_TEXTURE_2D, _tileTex[(unsigned)index - 10]);

					glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);

					//glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					glBindTexture(GL_TEXTURE_2D, 0);
					count++;
				}
			}
		}

		count = count + 1;
		RESOURCE->GetShader()->ExitShader();
	}


}