/******************************************************************************/
/*!
\file		SystemGraphics.h
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
#pragma once

#include "FwGraphics.h"
#include "ISystem.h"
#include <set>
#include <string>
#include "Framework.h"
#include "ShaderSystem.h"
#include "GraphicENUM.h"
#include <functional>
#include "Map.h"
#include "Math.h"
#include "ResourceManager.h"

namespace FwEngine
{
	typedef std::function<bool(ComponentRenderer*, ComponentRenderer*)> compare;

	//class ComponentCamera;

	class SystemGraphics : public ISystem, virtual public SystemObservable
	{
	private:
		HDC _windowDC;//global Window-device context
		HGLRC _openglDC;//OpenGL-device context
		HWND _handle;//handle to window

		int _resolutionWidth;
		int _resolutionHeight;

		float _ratioWidth;
		float _ratioHeight;

	public:
		enum DEBUG_STATE
		{
			DEBUG_STATE_OFF,
			DEBUG_STATE_SHOWALL,
			DEBUG_STATE_SHOWCOLLISION,
			DEBUG_STATE_SHOWVELOCITY,

			DEBUG_STATE_END
		};

		enum TEST_STATE
		{
			TRANSLATE,
			ROTATE,
			SCALE
		};
		SystemGraphics();
		//SystemGraphics(GameObjectPool* gameObjectPool) : ISystem(gameObjectPool) {}
		~SystemGraphics();


		//Camera stuff & windows stuff for editor
		//ComponentCamera* _currentCamera;
		FwMath::Vector3D _cameraPos;
		float _cameraRotation;
		float _near;
		float _far;

		//Projection only will change when screen size changes
		FwMath::Matrix4x4 _projection;
		FwMath::Matrix4x4 _UIProjection;
		//View changes when camera changes
		FwMath::Matrix4x4 _view;
		//Projection view matrix to store to lessen the load on matrix multiplication
		FwMath::Matrix4x4 _projectionView;
		float _zoom;

		//Simulate Layering, unless using z-axis for layering
		std::multiset <ComponentRenderer*, compare> _renderSet;
		std::vector<ComponentText*> _textVector;
		std::vector<ComponentRenderer*> _uiVector;

		//Map
		Map* _map;

		//Debug
		DEBUG_STATE _debugState;
		TEST_STATE _testState;
		bool _isPrintUI;

		HDC GetContext();
		HGLRC GetGLContext();
		HWND GetHandle();

		int GetWidth();
		int GetHeight();

		void InitMesh();
		void InitProjection();
		bool InitOpenGLRendering();
		void InitShaders();
		void InitCameraView();
		void SetGraphicsProperties(HWND handle, int width, int height);
		void AddRenderer(ComponentRenderer* render);
		void AddText(ComponentText* text);
		void DeleteRenderer(ComponentRenderer* render);
		void DeleteText(ComponentText* text);
		void UpdateCameraView();
		void UpdateZoom();
		void UpdateProjection(float newWidth, float newHeight);
		void UpdateProjectionView();

		void ChangeResolution(int width, int height);
		void LookAt(ComponentTransform* transform);

		//Assuming translation is alr calculated for pos
		void DrawDebugBox(FwMath::Vector3D pos, float size, FwMath::Vector3D colour);
		//Others drawing function for different shapes
		void DrawDebugBox(FwMath::Vector3D pos, FwMath::Vector3D size, FwMath::Vector3D colour);
		void DrawDebugCircle(FwMath::Vector3D pos, float radius, FwMath::Vector3D colour);
		void DrawDebugLine(glm::vec3 startPos, glm::vec3 endPos, FwMath::Vector3D colourStart, FwMath::Vector3D colourEnd);

		void DrawDebug();
		void RenderSprite(ComponentRenderer* render, float dt);
		void RenderText(ComponentText* text);
		void RenderUI(ComponentRenderer* ui, float dt);
		void RenderParticle(ComponentRenderer* particle, float dt);

		void UpdateAnimation(ComponentAnimation* animation, float dt);
		void UpdateParticles(ComponentEmitter* emitter, float dt);

		void Init() override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;
	};

	extern SystemGraphics* GRAPHICS;
}

