/******************************************************************************/
/*!
\file		SystemGraphics.cpp
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <iostream>
#include "SystemGraphics.h"
#include "DebugToStream.h"
#include "SOIL.h"
#include "WindowSystem.h"
#include "SystemInput.h"

#define DEBUG_VELOCITY_COLOUR 0.0f,1.0f,1.0f
#define DEBUG_COLLIDER_COLOUR 1.0f,1.0f,0.0f

#define DEBUG_MODE VK_HOME
#define TOGGLE_UI VK_ADD
#define SCREENSHOT VK_F12


namespace FwEngine
{
	SystemGraphics* GRAPHICS = nullptr;

	const auto less = [](ComponentRenderer* a, ComponentRenderer* b) { return a->_layer < b->_layer; };


	SystemGraphics::SystemGraphics() : _windowDC{ NULL }, _openglDC{ NULL }, _handle{ NULL }, 
		_resolutionWidth{ 0 }, _resolutionHeight{ 0 }, _ratioWidth{ 1.0f }, _ratioHeight{ 1.0f },
		_cameraPos{ }, _cameraRotation(0.0f), _near{ 0.0f }, _far{ 0.0f },
		_projection{ FwMath::Matrix4x4(1.0f) }, _UIProjection{ FwMath::Matrix4x4(1.0f) }, _view{ FwMath::Matrix4x4(1.0f) }, 
		_projectionView{ FwMath::Matrix4x4(1.0f) }, _renderSet(less), _textVector{}, _map{ nullptr }, 
		_debugState{ DEBUG_STATE_OFF }, _testState{ TRANSLATE }, _zoom{ 1.0f }, _isPrintUI{true}
	{
		GRAPHICS = this;
	}


	SystemGraphics::~SystemGraphics()
	{
		//Remember to destroy opengl when program is closing
		DestroyOpenGL(_handle, _windowDC, _openglDC);
	}

	void SystemGraphics::Init()
	{
		if (!InitOpenGLRendering())
		{
			throw 0;
		}
		//Initialize VAO VBO EBO which is for a square mesh.
		InitMesh();
		//Initialize projection matrix
		InitProjection();
		//Initialize view matrix
		InitCameraView();
		//Initialize the shader system
		InitShaders();
		//Update the projection view matrix
		UpdateProjectionView();

		//For transparency
		EnableBlendMode();
		BlendFunction();

		//Dk if this works can't tell
		ToggleVSync(true);
	}

	HDC SystemGraphics::GetContext()
	{
		return _windowDC;
	}

	HGLRC SystemGraphics::GetGLContext()
	{
		return _openglDC;
	}

	HWND SystemGraphics::GetHandle()
	{
		return _handle;
	}

	int SystemGraphics::GetWidth()
	{
		return _resolutionWidth;
	}

	int SystemGraphics::GetHeight()
	{
		return _resolutionHeight;
	}

	void SystemGraphics::InitMesh()
	{
		const FwMath::UINT numOfSides = 4;
		const FwMath::UINT dataPerVertex = 9;

		VertexData vertices[numOfSides] = {
			{
				-0.5f,//position
				0.5f,//position
				0.0f,//position
				0.0f,//texture
				0.0f,//texture
				1.0f,//Colour
				1.0f,//Colour
				1.0f,//Colour
				1.0f//Colour
			},
			{
				0.5f,//position
				0.5f,//position
				0.0f,//position
				1.0f,//texture
				0.0f,//texture
				1.0f,//Colour
				1.0f,//Colour
				1.0f,//Colour
				1.0f//Colour
			},
			{
				0.5f,//position
				-0.5f,//position
				0.0f,//position
				1.0f,//texture
				1.0f,//texture
				1.0f,//Colour
				1.0f,//Colour
				1.0f,//Colour
				1.0f//Colour
			},
			{
				-0.5f,//position
				-0.5f,//position
				0.0f,//position
				0.0f,//texture
				1.0f,//texture
				1.0f,//Colour
				1.0f,//Colour
				1.0f,//Colour
				1.0f//Colour
			}
		};

		// 0 1
		// 3 2
		FwMath::UINT indices[] = { 0,1,2,
							       2,3,0 };

		// x,y,z - ux,uy - r,g,b,a
		FwMath::UINT attribInfo[] = { 3, 2, 4 };

		//Initialize the mesh for sprites
		InitializeBuffers(RESOURCE->GetMesh(SPRITE), vertices, dataPerVertex, numOfSides, indices, std::size(indices), attribInfo, std::size(attribInfo),STATIC_DRAW);

		//Initialize the mesh for fonts
		InitializeBuffers(RESOURCE->GetMesh(FONT), nullptr, dataPerVertex, 6, nullptr, std::size(indices), attribInfo, std::size(attribInfo), DYNAMIC_DRAW);
		
		//Initialize the mesh for particle
		InitializeBuffers(RESOURCE->GetMesh(PARTICLES), nullptr, dataPerVertex, 6, nullptr, std::size(indices), attribInfo, std::size(attribInfo), DYNAMIC_DRAW);

	}

	void SystemGraphics::InitProjection()
	{
		_near = -10.0f;
		_far = 10.0f;
		_zoom = 1.0f;

		//Get the window width,height and calculate the ratio to resolution
		int width = WINDOWMAIN->GetWinWidth();
		int height = WINDOWMAIN->GetWinHeight();
		_ratioWidth = (float)_resolutionWidth / width;
		_ratioHeight = (float)_resolutionHeight / height;

		//Set the viewport
		Viewport(0, 0, width, height);

		//Init projection for other objects
		FwMath::Ortho(_projection, 
					 (float)-width * _ratioWidth * 0.5f * _zoom,
				     (float)width * _ratioWidth * 0.5f * _zoom,
					 (float)-height * _ratioHeight * 0.5f * _zoom,
					 (float)height * _ratioHeight * 0.5f * _zoom,
					 _near, 
					 _far);

		// Init projection for UI 
		FwMath::Ortho(_UIProjection,
			(float)-width * _ratioWidth * 0.5f * 1.0f,
			(float)width * _ratioWidth * 0.5f * 1.0f,
			(float)-height * _ratioHeight * 0.5f * 1.0f,
			(float)height * _ratioHeight * 0.5f * 1.0f,
			_near,
			_far);
	}

	bool SystemGraphics::InitOpenGLRendering()
	{
		return InitializeOpenGL(_handle, _windowDC, _openglDC);
	}

	void SystemGraphics::InitShaders()
	{
		try
		{
			RESOURCE->LoadShaders();
		}
		catch(ShaderSystem::ShaderException& error)
		{
			throw error;
		}
	}

	void SystemGraphics::InitCameraView()
	{
		//The camera current position
		_cameraPos = FwMath::Vector3D(0.0f, 0.0f, 10.0f);
		//The direction the camera is looking at
		FwMath::Vector3D lookAtPoint(_cameraPos.x, _cameraPos.y, 0.0f);
		//The up vector defines which way is up (the y-direction).
		FwMath::Vector3D upVector(0.0f, 1.0f, 0.0f);

		//The matrix for the MVP 
		FwMath::LookAt(_view, _cameraPos, lookAtPoint, upVector);
	}

	void SystemGraphics::SetGraphicsProperties(HWND handle, int width , int height)
	{
		//Set the resolution to run the game in
		_resolutionWidth = width;
		_resolutionHeight = height;
		_handle = handle;
	}

	void SystemGraphics::AddRenderer(ComponentRenderer* render)
	{
		//Search thru the multiset and insert if not exist, remove and insert is exist to reorder the renderer components
		auto it = std::find(_renderSet.begin(), _renderSet.end(), render);;
		if (it == _renderSet.end())
		{
			_renderSet.insert(render);
		}
		else
		{
			_renderSet.erase(it);
			_renderSet.insert(render);
		}
	}

	void SystemGraphics::AddText(ComponentText* text)
	{
		//Search thru the vector and insert if not exist
		auto it = std::find(_textVector.begin(), _textVector.end(), text);;
		if (it == _textVector.end())
		{
			_textVector.push_back(text);
		}
	}

	void SystemGraphics::DeleteRenderer(ComponentRenderer* render)
	{
		//Search thru the multiset and delete if exist
		auto it = std::find(_renderSet.begin(), _renderSet.end(), render);;
		if (it != _renderSet.end())
		{
			_renderSet.erase(it);
		}
	}

	void SystemGraphics::DeleteText(ComponentText* text)
	{
		//Search thru the vector and delete if exist
		auto it = std::find(_textVector.begin(), _textVector.end(), text);;
		if (it != _textVector.end())
		{
			_textVector.erase(it);
		}
	}

	void SystemGraphics::UpdateCameraView()
	{
		//Snap the camera position to the nearest whole number
		int x = (int)_cameraPos.x;
		int y = (int)_cameraPos.y;
		FwMath::Vector3D camPos((float)x, (float)y, 10.0f);
		_cameraPos = camPos;
		//The direction the camera is looking at
		FwMath::Vector3D lookAtPoint(_cameraPos.x, _cameraPos.y, 0.0f);
		// fake rotation (lol)
		//_cameraRotation = 0.1f;
		//The up vector defines which way is up (the y-direction).
		FwMath::Vector3D upVector(_cameraRotation, 1.0f, 0.0f);
		//view projection based on camera attributes
		
		//Get the view matrix based on camera pos, where its looking at and its up orientation
		FwMath::LookAt(_view, _cameraPos, lookAtPoint, upVector);

		//Update the new projection view matrix after camera movement
		UpdateProjectionView();
	}

	void SystemGraphics::UpdateZoom()
	{
		//Reset the projection matrix and recalculate it based on the new zoom value
		_projection = FwMath::Matrix4x4(1);
		FwMath::Ortho(_projection,
			(float)-WINDOWMAIN->GetWinWidth() * _ratioWidth * 0.5f * _zoom,
			(float)WINDOWMAIN->GetWinWidth() * _ratioWidth * 0.5f * _zoom,
			(float)-WINDOWMAIN->GetWinHeight() * _ratioHeight * 0.5f * _zoom,
			(float)WINDOWMAIN->GetWinHeight() * _ratioHeight * 0.5f * _zoom,
			_near,
			_far);

		//Update the new projection view matrix after camera zoom
		UpdateProjectionView();
	}

	void SystemGraphics::UpdateProjection(float newWidth, float newHeight)
	{
		//Set the new viewport and calculate the new ratio
		Viewport(0, 0, (int)newWidth, (int)newHeight);
		_ratioWidth = (float)_resolutionWidth / newWidth;
		_ratioHeight = (float)_resolutionHeight / newHeight;

		//Recalculate the projection matrix for both the ui and normal objects
		FwMath::Ortho(_projection,
			(float)-newWidth * _ratioWidth * 0.5f * _zoom,
			(float)newWidth * _ratioWidth * 0.5f * _zoom,
			(float)-newHeight * _ratioHeight * 0.5f * _zoom,
			(float)newHeight * _ratioHeight * 0.5f * _zoom,
			_near,
			_far);

		FwMath::Ortho(_UIProjection,
			(float)-newWidth * _ratioWidth * 0.5f,
			(float)newWidth * _ratioWidth * 0.5f,
			(float)-newHeight * _ratioHeight * 0.5f,
			(float)newHeight * _ratioHeight * 0.5f,
			_near,
			_far);

		//Update the new projection view matrix after screen resize
		UpdateProjectionView();
	}

	void SystemGraphics::UpdateProjectionView()
	{
		_projectionView = _projection * _view;
	}

	void SystemGraphics::ChangeResolution(int width, int height)
	{
		_resolutionWidth = width;
		_resolutionHeight = height;
	}

	void SystemGraphics::LookAt(ComponentTransform* transform)
	{
		_cameraPos = transform->_currentPosition;
		//The direction the camera is looking at
		FwMath::Vector3D lookAtPoint(_cameraPos.x, _cameraPos.y, 0.0f);
		//The up vector defines which way is up (the y-direction).
		FwMath::Vector3D upVector(0.0f, 1.0f, 0.0f);
		//view projection based on camera attributes
		FwMath::LookAt(_view, _cameraPos, lookAtPoint, upVector);

		//Update the new projection view matrix after camera movement
		UpdateProjectionView();
	}

	void SystemGraphics::DrawDebugBox(FwMath::Vector3D pos, float size, FwMath::Vector3D colour)
	{
		//Get the 4 corners
		const FwMath::UINT numOfSides = 4;
		FwMath::Vector4D boxPos[numOfSides] = {
			{pos.x - size * 0.5f, pos.y + size * 0.5f, 0.0f, 1.0f},
			{pos.x - size * 0.5f, pos.y - size * 0.5f, 0.0f, 1.0f},
			{pos.x + size * 0.5f, pos.y - size * 0.5f, 0.0f, 1.0f},
			{pos.x + size * 0.5f, pos.y + size * 0.5f, 0.0f, 1.0f}
		};

		//Use the projection view matrix to calculate the final position
		boxPos[0] = _projectionView * boxPos[0];
		boxPos[1] = _projectionView * boxPos[1];
		boxPos[2] = _projectionView * boxPos[2];
		boxPos[3] = _projectionView * boxPos[3];

		//Draw 4 lines
		for (int i = 0; i < numOfSides; ++i)
		{
			if ((i + 1) == 4)
			{
				DrawLine(boxPos[i], 
						 FwMath::Vector4D(colour.x,colour.y,colour.z,1.0f), 
						 boxPos[0], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f));
			}
			else
			{
				DrawLine(boxPos[i], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f), 
						 boxPos[i+1], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f));
			}
		}
	}

	void SystemGraphics::DrawDebugBox(FwMath::Vector3D pos, FwMath::Vector3D size, FwMath::Vector3D colour)
	{
		//Get the 4 corners
		const FwMath::UINT numOfSides = 4;
		FwMath::Vector4D boxPos[numOfSides] = {
			{pos.x - size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f, 1.0f},
			{pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, 0.0f, 1.0f},
			{pos.x + size.x * 0.5f, pos.y - size.y * 0.5f, 0.0f, 1.0f},
			{pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f, 1.0f}
		};

		//Use the projection view matrix to calculate the final position
		boxPos[0] = _projectionView * boxPos[0];
		boxPos[1] = _projectionView * boxPos[1];
		boxPos[2] = _projectionView * boxPos[2];
		boxPos[3] = _projectionView * boxPos[3];

		//Draw 4 lines
		for (int i = 0; i < numOfSides; ++i)
		{
			if ((i + 1) == 4)
			{
				DrawLine(boxPos[i], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f),
						 boxPos[0], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f));
			}
			else
			{
				DrawLine(boxPos[i], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f), 
						 boxPos[i+1], 
						 FwMath::Vector4D(colour.x, colour.y, colour.z, 1.0f));
			}
		}
	}

	void SystemGraphics::DrawDebugCircle(FwMath::Vector3D pos, float radius, FwMath::Vector3D colour)
	{
		const FwMath::UINT circleSegments = 16;
		const float increment = FwMath::DegreeToRadian(360.0f) / float(circleSegments);

		//Calculate the starting pos
		float theta = 0.0f;
		FwMath::Vector3D startingPos;
		FwMath::Vector3D endPos;
		startingPos = pos + radius * FwMath::Vector3D(FwMath::Cosine(theta), FwMath::Sine(theta), 0.0f);

		//Draw 16 lines
		for (unsigned i = 1; i <= circleSegments; ++i)
		{
			endPos = pos + radius * FwMath::Vector3D(FwMath::Cosine(increment * i), FwMath::Sine(increment * i), 0.0f);
			DrawDebugLine(startingPos, endPos, colour, colour);
			startingPos = endPos;
		}
	}

	void SystemGraphics::DrawDebugLine(glm::vec3 startPos, glm::vec3 endPos, FwMath::Vector3D colourStart, FwMath::Vector3D colourEnd)
	{
		FwMath::Vector4D pos1;
		FwMath::Vector4D pos2;

		//Calculate the final transform after projection view
		pos1 = _projectionView * FwMath::Vector4D(startPos, 1.0f);
		pos2 = _projectionView * FwMath::Vector4D(endPos, 1.0f);

		DrawLine(pos1, 
				 FwMath::Vector4D(colourStart.x, colourStart.y, colourStart.z, 1.0f), 
				 pos2, 
				 FwMath::Vector4D(colourEnd.x, colourEnd.y, colourEnd.z, 1.0f));
	}

	void SystemGraphics::DrawDebug()
	{
		ComponentPhysics* phyComp = &_gameObjectPool->GetContainerPtr<ComponentPhysics>()->at(0);
		ComponentCollision* colComp = &_gameObjectPool->GetContainerPtr<ComponentCollision>()->at(0);
		ComponentTransform* colTrans = &_gameObjectPool->GetContainerPtr<ComponentTransform>()->at(0);
		GameObject* gameObject = _gameObjectPool->GetGameObject(0);
		size_t size = _gameObjectPool->GetContainerPtr<ComponentRenderer>()->size();
		for (size_t i = 0; i < size; ++phyComp, ++colComp, ++colTrans, ++i)
		{
			// skip inactives
			if (!gameObject->_isActive)
				continue;

			// velocity
			if (phyComp->isEnabled() && colTrans->isEnabled() && ( _debugState == DEBUG_STATE_SHOWALL || _debugState == DEBUG_STATE_SHOWVELOCITY))
			{
				FwMath::Vector3D verticeA = colTrans->_currentPosition + phyComp->_velocity;
				DrawDebugLine(colTrans->_currentPosition, verticeA, FwMath::Vector3D(1.0f,1.0f,1.0f), FwMath::Vector3D(DEBUG_VELOCITY_COLOUR));
			}
			// collision
			if (colComp->isEnabled() && colTrans->isEnabled() && (_debugState == DEBUG_STATE_SHOWALL || _debugState == DEBUG_STATE_SHOWCOLLISION))
			{
				FwMath::Vector3D verticeA = colTrans->_currentPosition + colComp->_vertexA * colTrans->_scale;
				FwMath::Vector3D verticeB = colTrans->_currentPosition + colComp->_vertexB * colTrans->_scale;
				FwMath::Vector3D verticeC = colTrans->_currentPosition + colComp->_vertexC * colTrans->_scale;
				FwMath::Vector3D verticeD = colTrans->_currentPosition + colComp->_vertexD * colTrans->_scale;

				DrawDebugLine(verticeA, verticeC, FwMath::Vector3D(1.0f, 1.0f, 1.0f), FwMath::Vector3D(DEBUG_VELOCITY_COLOUR));
				DrawDebugLine(verticeC, verticeB, FwMath::Vector3D(1.0f, 1.0f, 1.0f), FwMath::Vector3D(DEBUG_VELOCITY_COLOUR));
				DrawDebugLine(verticeB, verticeD, FwMath::Vector3D(1.0f, 1.0f, 1.0f), FwMath::Vector3D(DEBUG_VELOCITY_COLOUR));
				DrawDebugLine(verticeD, verticeA, FwMath::Vector3D(1.0f, 1.0f, 1.0f), FwMath::Vector3D(DEBUG_VELOCITY_COLOUR));
			}

		}
	}

	void SystemGraphics::RenderSprite(ComponentRenderer* render, float dt)
	{
		//Declare some matrix and vector to use later
		FwMath::Matrix4x4 mvp(1);
		FwMath::Matrix4x4 model(1);
		FwMath::Matrix4x4 translateMat(1), rotateMat(1), scaleMat(1);
		FwMath::Vector3D scale;

		BlendFunction();

		//Checks for image flipping
		scale = render->GetTransform()->_scale;
		if (render->_flipX)
		{
			scale.x *= -1;
		}
		if (render->_flipY)
		{
			scale.y *= -1;
		}

		//Apply scaling and rotation into matrix
		FwMath::MtxScale(scaleMat, scale);
		FwMath::MtxRotation(rotateMat, render->GetTransform()->_rotation);

		//Gets rid of floating point value and set translation matrix
		int posX, posY, posZ;
		Vector3D tempPos = render->GetTransform()->_currentPosition;
		posX = (int)tempPos.x;
		posY = (int)tempPos.y;
		posZ = (int)tempPos.z;
		Vector3D currPos = Vector3D((float)posX, (float)posY, (float)posZ);
		render->GetTransform()->_currentPosition = currPos;
		FwMath::MtxTranslation(translateMat, currPos);

		//Calculate mvp
		model = translateMat * rotateMat * scaleMat;
		mvp = _projectionView * model;

		//If texture is set, renderer will be set to texture mode
		if (render->GetTexture())
		{
			render->SetShaderType(TEXTURE);
		}
		else
		{
			render->SetShaderType(COLOUR);
		}

		//Bind the renderer texture into the shader
		EnableTexture(render->GetTexture());

		//Checks for animation component
		if (render->GetAnimation())
		{
			if (render->GetAnimation()->isEnabled())
			{
				//Animation is enabled and therefore needed to be updated
				//Shader change to spritesheet state
				RESOURCE->GetShader()->ChangeShader(SPRITESHEET);
				RESOURCE->GetShader()->UseShader();
				UpdateAnimation(render->GetAnimation(), dt);
			}
			else
			{
				RESOURCE->GetShader()->ChangeShader(render->GetShaderType());
				RESOURCE->GetShader()->UseShader();
			}
		}
		else
		{
			RESOURCE->GetShader()->ChangeShader(render->GetShaderType());
			RESOURCE->GetShader()->UseShader();
		}

		//Send matrix and colour thru uniform
		RESOURCE->GetShader()->SetUniformMat4("MVP", mvp);
		RESOURCE->GetShader()->SetUniformVec4("Colour", render->_colour);

		//Get sprite mesh and draw it
		EnableMesh(RESOURCE->GetMesh(SPRITE));
		DrawSpriteElement();

		//Disable the mesh and disable texture
		DisableMesh();
		DisableTexture();

		//IMPORTANT: Remember to exit shader after using it every time
		RESOURCE->GetShader()->ExitShader();
	}

	void SystemGraphics::RenderText(ComponentText* text)
	{
		if (text->_displayText.empty() || text->GetFont() == nullptr)
		{
			return;
		}

		BlendFunction();

		FwMath::Matrix4x4 mvp(1);
		FwMath::Matrix4x4 model(1);

		FwMath::Matrix4x4 translateMat(1), rotateMat(1), scaleMat(1);
		FwMath::Vector3D scale;

		scale = text->GetTransform()->_scale;
		FwMath::MtxScale(scaleMat, scale);
		FwMath::MtxRotation(rotateMat, text->GetTransform()->_rotation);
		FwMath::MtxTranslation(translateMat, text->GetTransform()->_currentPosition);

		model = translateMat * rotateMat * scaleMat;
		mvp = _projectionView * model;

		// Activate corresponding render state	
		RESOURCE->GetShader()->ChangeShader(STRING);
		RESOURCE->GetShader()->UseShader();
		RESOURCE->GetShader()->SetUniformVec4("textColor", text->_colour);
		RESOURCE->GetShader()->SetUniformMat4("mvp", mvp);

		EnableMesh(RESOURCE->GetMesh(FONT));

		//For looping through the whole text
		auto currentChar = text->_displayText.begin();
		auto end = text->_displayText.end();

		//As transform x position shouldnt be changed in the process of rendering text, temporary x is used for storage
		float x = -(text->CalculateRowWidth(currentChar) * 0.5f);
		float y = (text->_pixel * (text->CountLines()-1) * 0.5f) - (text->_pixel * 0.5f);

		FontAtlas* characters = text->GetFont();

		//Get the string length and reserve the memory for faster batching
		std::vector<VertexData> textData;
		size_t textSize = text->_displayText.size();
		textData.reserve(textSize * 6);

		for (currentChar = text->_displayText.begin(); currentChar != end;)
		{
			if (!(*currentChar))
			{
				return;
			}

			if ((*currentChar) == 0x00 || (*currentChar) == 0x0A)
			{
				++currentChar;
				x = -((text->CalculateRowWidth(currentChar)) * 0.5f);
				y -= text->_pixel;
				continue;
			}

			if ((*currentChar) < 32 || (*currentChar) > 127)
			{
				++currentChar;
				continue;
			}

			CharInfo ch = characters->_chars[*currentChar];

			float xpos = x + ch._sizeLT.x;
			float ypos = y - (ch._sizeWH.y - ch._sizeLT.y);

			float w = (float)ch._sizeWH.x;
			float h = (float)ch._sizeWH.y;

			VertexData topLeft	{ xpos, ypos + h, 0.0f, //Position
								  ch._uvOffset.x, ch._uvOffset.y, //UV
								  1.0f, 1.0f, 1.0f, 1.0f };//Colour

			VertexData topRight	{ xpos + w, ypos + h, 0.0f, //Position
								  ch._uvOffset.x  + ((float)ch._sizeWH.x/characters->_width), ch._uvOffset.y, 	//UV
								  1.0f, 1.0f, 1.0f, 1.0f };//Colour

			VertexData btmLeft	{ xpos, ypos, 0.0f, //Position
								  ch._uvOffset.x, ch._uvOffset.y + ((float)ch._sizeWH.y/characters->_height),//UV
								  1.0f, 1.0f, 1.0f, 1.0f };//Colour

			VertexData btmRight	{ xpos + w, ypos, 0.0f, //Position
								  ch._uvOffset.x + ((float)ch._sizeWH.x / characters->_width), ch._uvOffset.y + ((float)ch._sizeWH.y / characters->_height),//UV
								  1.0f, 1.0f, 1.0f, 1.0f };//Colour

			textData.push_back(topLeft);
			textData.push_back(btmLeft);
			textData.push_back(btmRight);

			textData.push_back(topLeft);
			textData.push_back(btmRight);
			textData.push_back(topRight);

			x += ch._advance.x;
			y += ch._advance.y;
			++currentChar;
		}
		EnableTexture(characters->_textureID);
		EnableBuffer(RESOURCE->GetMesh(FONT));
		BufferData(sizeof(VertexData)* textData.size(), &textData[0]);
		DrawSpriteArray((int)textData.size());
		DisableBuffer();
		DisableMesh();
		DisableTexture();
		//VERY IMPORTANT !!! PLEASE REMEMBER TO EXIT SHADER AFTER DRAWING
		RESOURCE->GetShader()->ExitShader();
	}

	void SystemGraphics::RenderUI(ComponentRenderer* render, float dt)
	{
		//Declare some matrix and vector to use later
		FwMath::Matrix4x4 mvp(1);
		FwMath::Matrix4x4 model(1);
		FwMath::Matrix4x4 translateMat(1), rotateMat(1), scaleMat(1);
		FwMath::Vector3D scale;

		BlendFunction();

		//Checks for image flipping
		scale = render->GetTransform()->_scale;
		if (render->_flipX)
		{
			scale.x *= -1;
		}
		if (render->_flipY)
		{
			scale.y *= -1;
		}

		//For UI, the scale is based on a ratio of 0 - 1 to determine size with respect to the resolution size
		scale.x = _resolutionWidth * scale.x;
		scale.y = _resolutionHeight * scale.y;

		//Apply scaling and rotation into matrix
		FwMath::MtxScale(scaleMat, scale);
		FwMath::MtxRotation(rotateMat, render->GetTransform()->_rotation);

		//For UI, the position is based on a ratio of -0.5f - 0.5f to determine position with respect to the resolution size
		FwMath::Vector3D uiPosition = render->GetTransform()->_currentPosition;
		uiPosition.x = _resolutionWidth * uiPosition.x;
		uiPosition.y = _resolutionHeight * uiPosition.y;

		//Set translation matrix and calculate mvp
		FwMath::MtxTranslation(translateMat, uiPosition);
		model = translateMat * rotateMat * scaleMat;
		mvp = _UIProjection * model;

		//If texture is set, renderer will be set to texture mode
		if (render->GetTexture())
		{
			render->SetShaderType(TEXTURE);
		}
		else
		{
			render->SetShaderType(COLOUR);
		}

		//Bind the renderer texture into the shader
		EnableTexture(render->GetTexture());

		//Checks for animation component
		if (render->GetAnimation())
		{
			if (render->GetAnimation()->isEnabled())
			{
				//Animation is enabled and therefore needed to be updated
				//Shader change to spritesheet state
				RESOURCE->GetShader()->ChangeShader(SPRITESHEET);
				RESOURCE->GetShader()->UseShader();
				UpdateAnimation(render->GetAnimation(), dt);
			}
			else
			{
				RESOURCE->GetShader()->ChangeShader(render->GetShaderType());
				RESOURCE->GetShader()->UseShader();
			}
		}
		else
		{
			RESOURCE->GetShader()->ChangeShader(render->GetShaderType());
			RESOURCE->GetShader()->UseShader();
		}

		//Send matrix and colour thru uniform
		RESOURCE->GetShader()->SetUniformMat4("MVP", mvp);
		RESOURCE->GetShader()->SetUniformVec4("Colour", render->_colour);

		//Get sprite mesh and draw it
		EnableMesh(RESOURCE->GetMesh(SPRITE));
		DrawSpriteElement();

		//Disable the mesh and disable texture
		DisableMesh();
		DisableTexture();

		//IMPORTANT: Remember to exit shader after using it every time
		RESOURCE->GetShader()->ExitShader();
	}

	void SystemGraphics::RenderParticle(ComponentRenderer* particle, float dt)
	{
		//Only render if playing
		if (!particle->GetEmitter()->_play)
		{
			return;
		}
		//Declare some matrix and vector to use later
		FwMath::Matrix4x4 mvp(1);
		FwMath::Matrix4x4 model(1);

		BlendFunction(GL_ONE);

		//Updates the particle system first
		ComponentEmitter* emitter = particle->GetEmitter();
		UpdateParticles(emitter, dt);

		//Get the amount of max particles and reserve the memory
		std::vector<VertexData> particlesData;
		size_t particleSize = particle->GetEmitter()->_maxParticles;
		std::vector<Particle> particles = particle->GetEmitter()->GetParticles();

		particlesData.reserve(particleSize*6);

		for (size_t i = 0; i < particleSize; ++i)
		{
			if (!particle->GetEmitter()->IsAlive(i))
			{
				continue;
			}
			FwMath::Matrix4x4 translateMat(1), scaleMat(1);
			FwMath::Vector3D scale;
			FwMath::Vector3D pos;

			//4 corners
			FwMath::Vector4D topLeft = { -0.5f, 0.5f, 0.0f, 1.0f };
			FwMath::Vector4D topRight = { 0.5f, 0.5f, 0.0f, 1.0f };
			FwMath::Vector4D btmRight = { 0.5f, -0.5f, 0.0f, 1.0f };
			FwMath::Vector4D btmLeft = { -0.5f, -0.5f, 0.0f, 1.0f };

			float aliveRatio = particles[i]._timeAlive / particles[i]._lifespan;

			if (emitter->_follow)
			{
				pos = particles[i]._position + particle->GetTransform()->_currentPosition;
			}
			else
			{
				pos = particles[i]._position;
			}

			if (emitter->_sizeOverTime)
			{
				scale = aliveRatio * particles[i]._size;
			}
			else
			{
				scale = particles[i]._size;
			}

			translateMat = FwMath::Matrix4x4(1);

			//Calculate the actual position
			FwMath::MtxScale(scaleMat, scale);
			FwMath::MtxTranslation(translateMat, pos);
			model = translateMat * scaleMat;
			mvp = _projectionView * model;

			topLeft = mvp * topLeft;
			topRight = mvp * topRight;
			btmRight = mvp * btmRight;
			btmLeft = mvp * btmLeft;

			FwMath::Vector4D currColor;
			if (emitter->_colourOverTime)
			{
				currColor = ((1.0f - aliveRatio) * particles[i]._colourStart) + ((aliveRatio)*particles[i]._colourEnd);
			}
			else
			{
				currColor = particles[i]._colourEnd;
			}

			if (emitter->_fade)
			{
				currColor.a *= (1.0f - aliveRatio);
			}

			//Push into vector
			particlesData.push_back({ topLeft.x, topLeft.y, 0.0, 0.0, 0.0, currColor.x, currColor.y, currColor.z, currColor.w});
			particlesData.push_back({ btmLeft.x, btmLeft.y, 0.0, 0.0, 1.0, currColor.x, currColor.y, currColor.z, currColor.w });
			particlesData.push_back({ btmRight.x,btmRight.y, 0.0, 1.0, 1.0, currColor.x, currColor.y, currColor.z, currColor.w });
			particlesData.push_back({ topLeft.x, topLeft.y, 0.0, 0.0, 0.0, currColor.x, currColor.y, currColor.z, currColor.w });
			particlesData.push_back({ btmRight.x,btmRight.y, 0.0, 1.0, 1.0, currColor.x, currColor.y, currColor.z, currColor.w });
			particlesData.push_back({ topRight.x,topRight.y, 0.0, 1.0, 0.0, currColor.x, currColor.y, currColor.z, currColor.w });
		}

		// Activate corresponding render state	
		RESOURCE->GetShader()->ChangeShader(PARTICLE);
		RESOURCE->GetShader()->UseShader();

		if (particlesData.size() > 0)
		{
			EnableTexture(particle->GetTexture());
			EnableMesh(RESOURCE->GetMesh(PARTICLES));
			EnableBuffer(RESOURCE->GetMesh(PARTICLES));
			BufferData(sizeof(VertexData) * particlesData.size(), &particlesData[0]);
			DrawSpriteArray((int)particlesData.size());
			DisableMesh();
			DisableTexture();
		}
		//VERY IMPORTANT !!! PLEASE REMEMBER TO EXIT SHADER AFTER DRAWING
		RESOURCE->GetShader()->ExitShader();
	}

	void SystemGraphics::UpdateAnimation(ComponentAnimation* animation, float dt)
	{
		//Send the uniforms into the shader to calculate uv offset
		RESOURCE->GetShader()->SetUniformInt("maxColumn", animation->_maxColumn);
		RESOURCE->GetShader()->SetUniformInt("maxRow", animation->_maxRow);
		
		if (animation->_play)
		{
			//if animation is playing normally
			if (!animation->_reverse)
			{
				//Increase animation frame time
				animation->AddDeltaTime(dt);

				//If current time is more than set frame time, change frame
				if (animation->GetDeltaTime() >= animation->_frameTimes[(size_t)animation->GetAnimationIndex()])
				{
					//Set to the next frame
					animation->SetAnimationFrame(animation->GetAnimationFrame() + 1);

					//If there is a set animation end, end at that frame, else continue into end frame.
					if (animation->GetAnimationEnd())
					{
						if (animation->GetAnimationFrame() > animation->GetAnimationEnd())
						{
							//If is it meant to loop set to the starting frame
							if (animation->_loop)
							{
								animation->SetAnimationFrame(animation->GetAnimationStart());
							}
							else
							{
								animation->_play = false;
								animation->SetAnimationFrame(animation->GetAnimationEnd());
							}
						}
					}
					else
					{
						if (animation->GetAnimationFrame() >= animation->_animationsFrames[(size_t)animation->GetAnimationIndex()])
						{
							if (animation->_loop)
							{
								animation->SetAnimationFrame(animation->GetAnimationStart());
							}
							else
							{
								animation->_play = false;
								animation->SetAnimationFrame(animation->_animationsFrames[(size_t)animation->GetAnimationIndex()] - 1);
							}
						}
					}

					animation->ResetDeltaTime();
				}
			}
			//if animation is playing in reverse
			else
			{
				//Increase animation frame time
				animation->AddDeltaTime(dt);

				//If current time is more than set frame time, change frame
				if (animation->GetDeltaTime() >= animation->_frameTimes[(size_t)animation->GetAnimationIndex()])
				{
					//Set to the previous frame
					animation->SetAnimationFrame(animation->GetAnimationFrame() - 1);

					//If animation has a start and it reaches it, runs till the starting frame, else run till 0
					if (animation->GetAnimationStart())
					{
						if (animation->GetAnimationFrame() < animation->GetAnimationStart())
						{
							//If loops set is to the end frame
							if (animation->_loop)
							{
								if (animation->GetAnimationEnd())
								{
									animation->SetAnimationFrame(animation->GetAnimationEnd());
								}
								else
								{
									animation->SetAnimationFrame(animation->_animationsFrames[(size_t)animation->GetAnimationIndex()] - 1);
								}
							}
							else
							{
								animation->_play = false;
								animation->SetAnimationFrame(animation->GetAnimationStart());
							}
						}
					}
					else
					{
						if (animation->GetAnimationFrame() < 0)
						{
							if (animation->_loop)
							{
								if (animation->GetAnimationEnd())
								{
									animation->SetAnimationFrame(animation->GetAnimationEnd());
								}
								else
								{
									animation->SetAnimationFrame(animation->_animationsFrames[(size_t)animation->GetAnimationIndex()] - 1);
								}
							}
							else
							{
								animation->_play = false;
								animation->SetAnimationFrame(0);
							}
						}
					}

					animation->ResetDeltaTime();
				}
			}
		}
		//If animation is not playing search for default animation
		else
		{
			if (animation->_currentAnimationName.empty())
			{
				if (!animation->_defaultAnimationName.empty())
				{
					animation->PlayAnimation();
				}
			}
		}

		RESOURCE->GetShader()->SetUniformFloat("offsetX", animation->GetOffsetX() * (animation->GetAnimationFrame()));
		RESOURCE->GetShader()->SetUniformFloat("offsetY", animation->GetOffsetY() * (animation->GetAnimationIndex()));
	}

	void SystemGraphics::UpdateParticles(ComponentEmitter* emitter, float dt)
	{
		if(emitter->_play)
		{
			//Update the emitter time
			emitter->AddTime(dt);

			//Loop and update all particles that are alive
			size_t particleSize = emitter->_maxParticles;
			std::vector<Particle>& particles = emitter->GetParticles();
			for (size_t i = 0; i < particleSize; ++i)
			{
				//Don't update if its dead
				if (!emitter->IsAlive(i))
				{
					continue;
				}

				FwMath::Vector3D vel = particles[i]._velocity;
				float aliveRatio = particles[i]._timeAlive / particles[i]._lifespan;

				//Speed increase the longer it is alive
				if (emitter->_speedOverTime && !emitter->_velocityOverTime)
				{
					particles[i]._position += vel * particles[i]._speed * aliveRatio * dt;
				}
				//Gives velocity the longer it is alive
				else if (!emitter->_speedOverTime && emitter->_velocityOverTime)
				{
					vel += aliveRatio * emitter->_velocity;
					particles[i]._position += vel * particles[i]._speed * dt;
				}
				//Both
				else if (emitter->_speedOverTime && emitter->_velocityOverTime)
				{
					vel += aliveRatio * emitter->_velocity;
					particles[i]._position += vel * particles[i]._speed * aliveRatio * dt;
				}
				else
				{
					particles[i]._position += vel * particles[i]._speed * dt;
				}

				particles[i]._timeAlive += dt;		

				//Particle should be dead
				if (particles[i]._timeAlive >= particles[i]._lifespan)
				{
					emitter->DecreaseAlive();
					//Kills the particle
					emitter->SetAlive(i, false);
				}			
			}

			//Update emitter
			if (emitter->GetTimePassed() > emitter->_durationPerCycle && !emitter->_loop)
			{
				//Don't Spawn Anymore
				if (emitter->GetAlive() == 0)
				{
					emitter->Stop();
				}
			}
			//Duration still within time passed or it is looping
			else
			{
				if (emitter->GetEmitterTime() > emitter->_emissionRate)
				{
					if (emitter->_burst)
					{
						emitter->ResetEmitterTime();
						emitter->InitParticles();
					}
					else
					{
						emitter->ResetEmitterTime();
						emitter->RespawnParticle();
					}
				}
			}
		}
	}

	void SystemGraphics::Update(float dt)
	{
#ifdef _LEVEL_EDITOR
		ClearScreen(FwMath::Vector4D(0.5f, 0.5f, 0.5f, 1.0f));
#else
		//To hide background
		ClearScreen(FwMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f));
#endif

		bool mapDrawn = false;
		//bool textDrawn = false;

		//Calculate the screen bound
		float botBound = _cameraPos.y - (_resolutionHeight * 0.5f) * _zoom;
		float topBound = _cameraPos.y + (_resolutionHeight * 0.5f) * _zoom;
		float rightBound = _cameraPos.x + (_resolutionWidth * 0.5f) * _zoom;
		float leftBound = _cameraPos.x - (_resolutionWidth * 0.5f) * _zoom;

		//Calculate the screen bound for particle
		//Allow space for particle with high velocity
		//float botBoundParticle = _cameraPos.y - (_resolutionHeight) * _zoom;
		//float topBoundParticle = _cameraPos.y + (_resolutionHeight) * _zoom;
		//float rightBoundParticle = _cameraPos.x + (_resolutionWidth) * _zoom;
		//float leftBoundParticle = _cameraPos.x - (_resolutionWidth) * _zoom;

		_uiVector.reserve(_renderSet.size());

		for (ComponentRenderer* i : _renderSet)
		{
			if (!i->getParentGameObject()
				|| !i->isEnabled()
				|| !i->getParentGameObject()->_isActive)
				continue;

			//If it a UI push into vector and continue
			if (i->_ui)
			{
				_uiVector.push_back(i);
				continue;
			}

			//Checks for boundary
			float posX = i->GetTransform()->_currentPosition.x;
			float posY = i->GetTransform()->_currentPosition.y;
			float scaleMax = i->GetTransform()->_scale.x > i->GetTransform()->_scale.y ? 
								i->GetTransform()->_scale.x : 
								i->GetTransform()->_scale.y;


			//If it is a particle render particle
			if (i->GetEmitter()->isEnabled())
			{
				/*ComponentEmitter* emit = i->GetEmitter();
				float maxDistance = emit->_maxParticleSpeed * emit->_maxLifespan;
				//Object is out of screen by about 2 screen away do not draw
				if (((((posX + scaleMax + maxDistance) < (leftBoundParticle) ||
					(posY + scaleMax + maxDistance) < (botBoundParticle) ||
					(posX - scaleMax + maxDistance) > (rightBoundParticle) ||
					(posY - scaleMax + maxDistance) > (topBoundParticle)))))
				{
					continue;
				}*/ //Might not need
				RenderParticle(i, dt);
				continue;
			}

			//Object is out do not draw
			if (((((posX + scaleMax) < leftBound ||
				   (posY + scaleMax) < botBound ||
				   (posX - scaleMax) > rightBound ||
				   (posY - scaleMax) > topBound))))
			{
				continue;
			}

			//Draws the tile map at layer 0
			if (!mapDrawn && i->_layer >= 0)
			{
				if (_map)
				{
					_map->Draw(topBound, botBound, leftBound, rightBound);
					mapDrawn = true;
				}
			}

			RenderSprite(i, dt);
		}

		//Draw map if hasnt been drawn
		if (!mapDrawn && _map != nullptr)
		{
			_map->Draw(topBound, botBound, leftBound, rightBound);
			mapDrawn = true;
		}

		//Render fonts
		for (ComponentText* t : _textVector)
		{
			if (!t->getParentGameObject()
				|| !t->isEnabled()
				|| !t->getParentGameObject()->_isActive)
			{
				continue;
			}
			float textX = t->GetTransform()->_currentPosition.x;
			float textY = t->GetTransform()->_currentPosition.y;
			float textMax = t->GetTransform()->_scale.x > t->GetTransform()->_scale.y ?
				t->GetTransform()->_scale.x :
				t->GetTransform()->_scale.y;
			float maxLength = t->CalculateMaxRowWidth() > (t->_pixel * t->CountLines()) ?
				t->CalculateMaxRowWidth() :
				t->_pixel * t->CountLines();

			//If outside of camera don't render
			if ((textX + textMax * maxLength) < leftBound ||
				(textY + textMax * maxLength) < botBound ||
				(textX - textMax * maxLength) > rightBound ||
				(textY - textMax * maxLength) > topBound)
			{
				continue;
			}

			RenderText(t);
		}

		//Draws UI if allowed to
		if (_isPrintUI)
		{
			for (ComponentRenderer* i : _uiVector)
			{
				RenderUI(i, dt);
			}
		}

		//Keys to trigger ui draw state
		if (GetAsyncKeyState(TOGGLE_UI) & 0x1)
		{
			_isPrintUI = !_isPrintUI;
		}

		if (GetAsyncKeyState(SCREENSHOT) & 0x1)
		{
			SaveScreenshot();
		}

		// temp toggle debug state
		if (GetAsyncKeyState(DEBUG_MODE) & 0x1)
		{
			_debugState = (DEBUG_STATE)(_debugState + 1);
			if (_debugState == DEBUG_STATE_END)
				_debugState = DEBUG_STATE_OFF;
		}

		if (_debugState != DEBUG_STATE_OFF)
		{
			DrawDebug();
		}

		_uiVector.clear();

#ifndef _LEVEL_EDITOR // Defined in Level Editor Project - Does nothing in Main Files
		ChangeBuffers(_windowDC); //using double buffering
#endif
	}

	void SystemGraphics::Free()
	{
		//std::cout << "SystemAnimation free" << std::endl;
		_textVector.clear();
		_renderSet.clear();
		_uiVector.clear();

		//std::cout << "SystemScript free" << std::endl;
		ComponentRenderer* compRender = &_gameObjectPool->GetContainerPtr<ComponentRenderer>()->at(0);
		ComponentText* compText = &_gameObjectPool->GetContainerPtr<ComponentText>()->at(0);
		ComponentAnimation* compAnim = &_gameObjectPool->GetContainerPtr<ComponentAnimation>()->at(0);
		ComponentEmitter* compEmitter = &_gameObjectPool->GetContainerPtr<ComponentEmitter>()->at(0);
		//GameObject* gameObject = _gameObjectPool->GetGameObject(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentRenderer>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++compRender, ++compText, ++compAnim, ++compEmitter)
		{
			compRender->disable();
			compRender->Free();
			compEmitter->disable();
			compEmitter->Free();
			compText->disable();
			compText->Free();
			compAnim->disable();
			compAnim->Free();
		}
	}

	void SystemGraphics::Destroy()
	{
		//std::cout << "SystemAnimation destroy" << std::endl;
	}

}
