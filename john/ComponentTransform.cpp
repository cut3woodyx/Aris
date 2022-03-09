/******************************************************************************/
/*!
\file		ComponentTransform.cpp
\project	Aris
\author 	Primary: Wang YiDi
\par    	email: w.yidi\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <iostream>

#include "ComponentTransform.h"
#include "GameObject.h"
#include "GameObjectPool.h"


constexpr auto STRING_TRANSFORM_TAG = "tag";

constexpr auto STRING_POSITION_X = "position_X";
constexpr auto STRING_POSITION_Y = "position_Y";
constexpr auto STRING_POSITION_Z = "position_Z";
constexpr auto STRING_SCALE_X = "scale_X";
constexpr auto STRING_SCALE_Y = "scale_Y";
constexpr auto STRING_ROTATION = "rotation";

namespace FwEngine
{
	ComponentTransform::ComponentTransform() : ComponentTransform(nullptr)
	{
	}

	ComponentTransform::ComponentTransform(GameObject* parent) : 
		IComponent(parent), _rotation{ 0 }, _scale{}, _currentPosition{}, _tag{}
	{
	}


	ComponentTransform::~ComponentTransform()
	{
	}


	void ComponentTransform::Init(ParamValueMap& paramValues)
	{
		//std::cout << "ComponentTransform init" << std::endl;

		_isEnabled = true;

		_tag = GetFirst(paramValues, STRING_TRANSFORM_TAG);

		_currentPosition = FwMath::Vector3D(
			std::stof(GetFirst(paramValues, STRING_POSITION_X)),
			std::stof(GetFirst(paramValues, STRING_POSITION_Y)),
			std::stof(GetFirst(paramValues, STRING_POSITION_Z))
		);

		_scale = FwMath::Vector3D(
			std::stof(GetFirst(paramValues, STRING_SCALE_X)),
			std::stof(GetFirst(paramValues, STRING_SCALE_Y)),
			0
		);

		_rotation = std::stof(GetFirst(paramValues, STRING_ROTATION));
	}

	void ComponentTransform::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentTransform& source = reinterpret_cast<ComponentTransform&>(sauce);

		_tag = source._tag;
		_currentPosition = source._currentPosition;
		_scale = source._scale;
		_rotation = source._rotation;
	}

	void ComponentTransform::Update(float )
	{
		//std::cout << "ComponentTransform update" << std::endl;
	}

	void ComponentTransform::Free()
	{
		//std::cout << "ComponentTransform free" << std::endl;
	}

	void ComponentTransform::Destroy()
	{
		//std::cout << "ComponentTransform destroy" << std::endl;
	}

	std::pair<std::string, ParamValueMap> ComponentTransform::GetParams()
	{
		ParamValueMap params;


		params.emplace(STRING_TRANSFORM_TAG, _tag);

		params.emplace(STRING_POSITION_X, std::to_string(_currentPosition.x));
		params.emplace(STRING_POSITION_Y, std::to_string(_currentPosition.y));
		params.emplace(STRING_POSITION_Z, std::to_string(_currentPosition.z));

		params.emplace(STRING_SCALE_X, std::to_string(_scale.x));
		params.emplace(STRING_SCALE_Y, std::to_string(_scale.y));

		params.emplace(STRING_ROTATION, std::to_string(_rotation));

		return { STRING_COMPONENT_TRANSFORM, params };
	}
}

