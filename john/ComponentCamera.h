/******************************************************************************/
/*!
\file		ComponentCamera.h
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

#include "IComponent.h"
#include "Component.h"
#include "FWMath.h"
#include "ComponentTransform.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_CAMERA = "camera";

	class ComponentCamera : public IComponent
	{
		//Private if not gonna appear on level editor
	private:
		ComponentTransform* _componentTransform;

		int _width;
		int _height;
		float _ratioWidth;
		float _ratioHeight;

		FwMath::Matrix4x4 _projection;
		FwMath::Matrix4x4 _view;
		FwMath::Matrix4x4 _projectionView;

	public:
		FwMath::Vector4D _bgColour;

		float _size;
		float _near;
		float _far;

		//From 0 to 1
		struct View
		{
			float _x = 0.0f;
			float _y = 0.0f;
			float _width = 1.0f;
			float _height = 1.0f;
		};

		View _viewport;

		ComponentCamera();
		ComponentCamera(GameObject* parent);
		~ComponentCamera();

		ComponentTransform* GetTransform();
		void SetTransform(ComponentTransform* transform);

		void UpdateProjection(int width, int height, float ratioWidth = 1.0f, float ratioHeight = 1.0f);
		void UpdateView();
		void UpdateZoom();
		FwMath::Matrix4x4& GetProjectionView();
		FwMath::Matrix4x4& GetProjection();

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}
