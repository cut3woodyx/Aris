/******************************************************************************/
/*!
\file		ComponentCamera.cpp
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
#include "ComponentCamera.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "ComponentTransform.h"

constexpr auto STRING_BGCOLOUR_RED = "red";
constexpr auto STRING_BGCOLOUR_GREEN = "green";
constexpr auto STRING_BGCOLOUR_BLUE = "blue";
constexpr auto STRING_BGCOLOUR_ALPHA = "alpha";
constexpr auto STRING_SIZE = "size";
constexpr auto STRING_NEAR = "near";
constexpr auto STRING_FAR = "far";
constexpr auto STRING_VIEWPORT_X = "viewport_x";
constexpr auto STRING_VIEWPORT_Y = "viewport_y";
constexpr auto STRING_VIEWPORT_W = "viewport_w";
constexpr auto STRING_VIEWPORT_H = "viewport_h";

namespace FwEngine
{
	ComponentCamera::ComponentCamera() : _componentTransform{ nullptr },
		_width{ 0 }, _height{ 0 }, _ratioWidth{ 1.0f }, _ratioHeight{ 1.0f },
		_projection{ FwMath::Matrix4x4(1.0f) }, _view{ FwMath::Matrix4x4(1.0f) },
		_projectionView{ FwMath::Matrix4x4(1.0f) },  _bgColour{ 1.0f,1.0f,1.0f,1.0f }, 
		_size{ 1.0f }, _near{ -10.0f }, _far{ 10.0f }, _viewport{}
	{
	}

	ComponentCamera::ComponentCamera(GameObject* parent) : IComponent(parent), 
		_componentTransform{ nullptr }, _width{ 0 }, _height{ 0 }, 
		_ratioWidth{ 1.0f }, _ratioHeight{ 1.0f }, _projection{ FwMath::Matrix4x4(1.0f) }, 
		_view{ FwMath::Matrix4x4(1.0f) }, _projectionView{ FwMath::Matrix4x4(1.0f) }, 
		_bgColour{ 0.0f,0.0f,0.0f,0.0f }, _size{ 1.0f }, _near{ -10.0f }, _far{ 10.0f },
		_viewport{}
	{
	}

	ComponentCamera::~ComponentCamera()
	{
	}

	ComponentTransform* ComponentCamera::GetTransform()
	{
		if (_componentTransform == nullptr)
		{
			_componentTransform = 
				&_parentGameObject->GetParentObjectPool()->
				GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		}

		return _componentTransform;
	}

	void ComponentCamera::SetTransform(ComponentTransform* transform)
	{
		_componentTransform = transform;
	}

	void ComponentCamera::UpdateProjection(int width, int height, float ratioWidth, float ratioHeight)
	{
		_width = width;
		_height = height;
		_ratioWidth = ratioWidth;
		_ratioHeight = ratioHeight;

		FwMath::Ortho(_projection,
			(float)-width * ratioWidth * 0.5f * _size,
			(float)width * ratioWidth * 0.5f * _size,
			(float)-height * ratioHeight * 0.5f * _size,
			(float)height * ratioHeight * 0.5f * _size,
			_near,
			_far);

		_projectionView = _projection * _view;
	}

	void ComponentCamera::UpdateView()
	{
		//The direction the camera is looking at
		FwMath::Vector3D lookAtPoint(_componentTransform->_currentPosition.x, _componentTransform->_currentPosition.y, 0.0f);
		//The up vector defines which way is up (the y-direction).
		FwMath::Vector3D upVector(0.0f, 1.0f, 0.0f);
		_componentTransform->_currentPosition.z = 10.0f;
		//The matrix for the MVP 
		FwMath::LookAt(_view, _componentTransform->_currentPosition, lookAtPoint, upVector);

		_projectionView = _projection * _view;
	}

	void ComponentCamera::UpdateZoom()
	{
		FwMath::Ortho(_projection,
			(float)-_width * _ratioWidth * 0.5f * _size,
			(float)_width * _ratioWidth * 0.5f * _size,
			(float)-_height * _ratioHeight * 0.5f * _size,
			(float)_height * _ratioHeight * 0.5f * _size,
			_near,
			_far);

		_projectionView = _projection * _view;
	}

	FwMath::Matrix4x4& ComponentCamera::GetProjectionView()
	{
		return _projectionView;
	}

	FwMath::Matrix4x4& ComponentCamera::GetProjection()
	{
		return _projection;
	}

	void ComponentCamera::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		float alpha = 1.0f;
		if (paramValues.find(STRING_BGCOLOUR_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_BGCOLOUR_ALPHA));
		}

		_bgColour = FwMath::Vector4D(std::stof(GetFirst(paramValues, STRING_BGCOLOUR_RED)),
									std::stof(GetFirst(paramValues, STRING_BGCOLOUR_GREEN)),
									std::stof(GetFirst(paramValues, STRING_BGCOLOUR_BLUE)),
									alpha);

		if (paramValues.find(STRING_SIZE) != paramValues.end())
		{
			_size = std::stof(GetFirst(paramValues, STRING_SIZE));
		}
		if (paramValues.find(STRING_NEAR) != paramValues.end())
		{
			_near = std::stof(GetFirst(paramValues, STRING_NEAR));
		}
		if (paramValues.find(STRING_FAR) != paramValues.end())
		{
			_far = std::stof(GetFirst(paramValues, STRING_FAR));
		}

		_viewport._x = std::stof(GetFirst(paramValues, STRING_VIEWPORT_X));
		_viewport._y = std::stof(GetFirst(paramValues, STRING_VIEWPORT_Y));

		if (paramValues.find(STRING_VIEWPORT_W) != paramValues.end())
		{
			_viewport._width = std::stof(GetFirst(paramValues, STRING_VIEWPORT_W));
		}
		if (paramValues.find(STRING_VIEWPORT_H) != paramValues.end())
		{
			_viewport._height = std::stof(GetFirst(paramValues, STRING_VIEWPORT_H));
		}

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
	}

	void ComponentCamera::Clone(IComponent& sauce)
	{

		_isEnabled = sauce.isEnabled();

		ComponentCamera& source = reinterpret_cast<ComponentCamera&>(sauce);

		_bgColour = source._bgColour;
		_size = source._size;
		_near = source._near;
		_far = source._far;
		_viewport = source._viewport;

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
	}

	void ComponentCamera::Update(float)
	{
	}

	void ComponentCamera::Free()
	{
	}

	void ComponentCamera::Destroy()
	{
	}

	std::pair<std::string, FwEngine::ParamValueMap> ComponentCamera::GetParams()
	{
		ParamValueMap params;

		params.emplace(STRING_BGCOLOUR_RED, std::to_string(_bgColour.x));
		params.emplace(STRING_BGCOLOUR_GREEN, std::to_string(_bgColour.y));
		params.emplace(STRING_BGCOLOUR_BLUE, std::to_string(_bgColour.z));
		params.emplace(STRING_BGCOLOUR_ALPHA, std::to_string(_bgColour.w));
		params.emplace(STRING_SIZE, std::to_string(_size));
		params.emplace(STRING_NEAR, std::to_string(_near));
		params.emplace(STRING_FAR, std::to_string(_far));
		params.emplace(STRING_VIEWPORT_X, std::to_string(_viewport._x));
		params.emplace(STRING_VIEWPORT_Y, std::to_string(_viewport._y));
		params.emplace(STRING_VIEWPORT_W, std::to_string(_viewport._width));
		params.emplace(STRING_VIEWPORT_H, std::to_string(_viewport._height));

		return { STRING_COMPONENT_CAMERA, params };
	}

}
