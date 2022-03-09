/******************************************************************************/
/*!
\file		ComponentUI.cpp
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
#include "ComponentUI.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "SystemGraphics.h"

namespace FwEngine
{
	constexpr auto STRING_TEXTURE = "texture";
	constexpr auto STRING_LAYER = "layer";
	constexpr auto STRING_COLOUR_RED = "red";
	constexpr auto STRING_COLOUR_GREEN = "green";
	constexpr auto STRING_COLOUR_BLUE = "blue";
	constexpr auto STRING_COLOUR_ALPHA = "alpha";

	ComponentUI::ComponentUI() : _textureID { 0 }, _componentTransform{ nullptr },
		_texture{ " " }, _layer{ 0 }, _colour{ 1.0f,1.0f,1.0f,1.0f }
	{
	}

	ComponentUI::ComponentUI(GameObject* parent) : IComponent(parent), _textureID{ 0 }, _componentTransform{ nullptr },
		_texture{ " " }, _layer{ 0 },_colour{ 1.0f,1.0f,1.0f,1.0f }
	{
	}
	ComponentUI::~ComponentUI()
	{
	}

	//Use SetTexture to ensure loading of texture as you change textures
	void ComponentUI::SetTexture(const std::string& newTexture)
	{
		_texture = newTexture;
		try
		{
			_textureID = RESOURCE->GetTexture(_texture);
		}
		catch (...)
		{
			_textureID = 0;
		}
	}

	FwMath::UINT ComponentUI::GetTexture()
	{
		return _textureID;
	}

	ComponentTransform* ComponentUI::GetTransform()
	{
		if (!_componentTransform)
		{
			_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		}

		return _componentTransform;
	}

	void ComponentUI::SetTransform(ComponentTransform* transform)
	{
		_componentTransform = transform;
	}

	void ComponentUI::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		_texture = GetFirst(paramValues, STRING_TEXTURE);
		_layer = std::stoi(GetFirst(paramValues, STRING_LAYER));

		//Texture filename might not be valid
		try
		{
			_textureID = RESOURCE->GetTexture(_texture);
		}
		catch (...)
		{
			//CDEBUG_PRINT(std::string(_texture).append(" not found."));
			_textureID = 0;
		}

		float red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;

		if (paramValues.find(STRING_COLOUR_RED) != paramValues.end())
		{
			red = std::stof(GetFirst(paramValues, STRING_COLOUR_RED));
		}
		if (paramValues.find(STRING_COLOUR_GREEN) != paramValues.end())
		{
			green = std::stof(GetFirst(paramValues, STRING_COLOUR_GREEN));
		}
		if (paramValues.find(STRING_COLOUR_BLUE) != paramValues.end())
		{
			blue = std::stof(GetFirst(paramValues, STRING_COLOUR_BLUE));
		}
		if (paramValues.find(STRING_COLOUR_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_COLOUR_ALPHA));
		}

		_colour = FwMath::Vector4D(red, green, blue, alpha);

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
	}

	void ComponentUI::Update(float)
	{

	}

	void ComponentUI::Free()
	{

	}

	void ComponentUI::Destroy()
	{

	}

	std::pair<std::string, ParamValueMap> ComponentUI::GetParams()
	{
		ParamValueMap params;

		params.emplace(STRING_TEXTURE, _texture);
		params.emplace(STRING_LAYER, std::to_string(_layer));
		params.emplace(STRING_COLOUR_RED, std::to_string(_colour.x));
		params.emplace(STRING_COLOUR_GREEN, std::to_string(_colour.y));
		params.emplace(STRING_COLOUR_BLUE, std::to_string(_colour.z));
		params.emplace(STRING_COLOUR_ALPHA, std::to_string(_colour.w));

		return { STRING_COMPONENT_RENDERER, params };
	}
}