/******************************************************************************/
/*!
\file		Map.h
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
#pragma once

#include <iostream>
#include "ShaderSystem.h"
#include <array>
#include <set>

namespace FwEngine
{
	class SystemGraphics;
	class Map
	{
	public:
		//Collision flags
		const unsigned int	COLLISION_LEFT = 0x00000001;	//0001
		const unsigned int	COLLISION_RIGHT = 0x00000002;	//0010
		const unsigned int	COLLISION_TOP = 0x00000004;	//0100
		const unsigned int	COLLISION_BOTTOM = 0x00000008;	//1000

		float MAP_SCALE = 100.0f;
		float MAP_SCALE_HF = MAP_SCALE * 0.5f;
		float MAP_SCALE_INVERSE = 1.0f / MAP_SCALE;
		float MAP_SCALE_P = MAP_SCALE / 10000.0f;

		std::vector<int> _mapArray;
		std::vector<int> _mapArrayCol;

		//int* _mapArray;
		int _height;
		int _width;

		int _noColIndex;

		std::vector<GLuint> _tileTex;
		//GLuint _tileTex[10];


		Map();
		~Map();

		void Init(std::string fileName);

		void InitTex(std::vector<std::string> textures);

		void Free();

		int GetCell(int width, int height);

		int GetCellCol(int width, int height);

		int CheckCellCollision(float PosX, float PosY, float xLeft, float xRight, float yBot, float yTop);

		float SnapToCell(float current, float scale, int flag);

		void Draw(float topBound, float botBound, float leftBound, float rightBound);

	};
}