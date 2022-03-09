/******************************************************************************/
/*!
\file		ComponentRenderer.cpp
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

#include "ComponentRenderer.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "SystemGraphics.h"


constexpr auto STRING_TYPE = "type";
constexpr auto STRING_TEXTURE = "texture";
constexpr auto STRING_LAYER = "layer";
constexpr auto STRING_COLOUR_RED = "red";
constexpr auto STRING_COLOUR_GREEN = "green";
constexpr auto STRING_COLOUR_BLUE = "blue";
constexpr auto STRING_COLOUR_ALPHA = "alpha";
constexpr auto STRING_FLIPX = "flipX";
constexpr auto STRING_FLIPY = "flipY";
constexpr auto STRING_UI = "ui";

namespace FwEngine
{
	ComponentRenderer::ComponentRenderer() : _textureID{ 0 }, _type{ TOTAL_SHADER }, 
		_componentTransform{ nullptr }, _componentAnimation{ nullptr }, _componentEmitter{ nullptr },
		_texture{ "" }, _layer{ 0 }, _colour{ 1.0f,1.0f,1.0f,1.0f }, _flipX{ false }, _flipY{ false }, _ui{ false }
	{
	}

	ComponentRenderer::ComponentRenderer(GameObject* parent) : IComponent(parent), _textureID{ 0 }, _type{ TOTAL_SHADER },
		_componentTransform{ nullptr }, _componentAnimation{ nullptr }, _componentEmitter{ nullptr },
		_texture{ "" }, _layer{ 0 }, _colour{ 1.0f,1.0f,1.0f,1.0f }, _flipX{ false }, _flipY{ false }, _ui{ false }
	{
	}


	ComponentRenderer::~ComponentRenderer()
	{
	}


	void ComponentRenderer::SetTexture(const std::string& newTexture)
	{
		_texture = newTexture;
		try
		{
			_textureID = RESOURCE->GetTexture(_texture);
			_type = TEXTURE;
		}
		catch (...)
		{
			_textureID = 0;
			_type = COLOUR;
		}
	}

	FwMath::UINT ComponentRenderer::GetTexture()
	{
		return _textureID;
	}

	ComponentTransform* ComponentRenderer::GetTransform()
	{
		if (!_componentTransform)
		{
			_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		}

		return _componentTransform;
	}

	ComponentAnimation* ComponentRenderer::GetAnimation()
	{
		if (!_componentAnimation)
		{
			_componentAnimation = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentAnimation>()->at(_parentGameObject->GetIndex());
		}

		return _componentAnimation;
	}

	ComponentEmitter* ComponentRenderer::GetEmitter()
	{
		if (!_componentEmitter)
		{
			_componentEmitter = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentEmitter>()->at(_parentGameObject->GetIndex());
		}

		return _componentEmitter;
	}

	void ComponentRenderer::SetTransform(ComponentTransform* transform)
	{
		_componentTransform = transform;
	}

	void ComponentRenderer::SetAnimation(ComponentAnimation* animation)
	{
		_componentAnimation = animation;
	}

	void ComponentRenderer::SetEmitter(ComponentEmitter* emitter)
	{
		_componentEmitter = emitter;
	}

	ShaderType ComponentRenderer::GetShaderType()
	{
		return _type;
	}

	void ComponentRenderer::SetShaderType(ShaderType type)
	{
		_type = type;
	}

	void ComponentRenderer::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		_layer = std::stoi(GetFirst(paramValues, STRING_LAYER));
		_texture = GetFirst(paramValues, STRING_TEXTURE);

		//Texture filename might not be valid
		try
		{
			_textureID = RESOURCE->GetTexture(_texture);
			_type = TEXTURE;
		}
		catch (...)
		{
			_textureID = 0;
			_type = COLOUR;
		}

		float red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;

		//Checks if colour saves exist, if not use default
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

		_flipX = std::stoi(GetFirst(paramValues, STRING_FLIPX));
		_flipY = std::stoi(GetFirst(paramValues, STRING_FLIPY));
		_ui = std::stoi(GetFirst(paramValues, STRING_UI));

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		_componentAnimation = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentAnimation>()->at(_parentGameObject->GetIndex());
		_componentEmitter = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentEmitter>()->at(_parentGameObject->GetIndex());
	}

	void ComponentRenderer::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentRenderer& source = reinterpret_cast<ComponentRenderer&>(sauce);

		_layer = source._layer;
		_texture = source._texture;
		_textureID = source._textureID;
		_type = source._type;

		_colour = source._colour;

		_flipX = source._flipX;
		_flipY = source._flipY;
		_ui = source._ui;

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		_componentAnimation = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentAnimation>()->at(_parentGameObject->GetIndex());
		_componentEmitter = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentEmitter>()->at(_parentGameObject->GetIndex());
	}

	void ComponentRenderer::Update(float)
	{
	}

	void ComponentRenderer::Free()
	{
	}

	void ComponentRenderer::Destroy()
	{
	}
	std::pair<std::string, ParamValueMap> ComponentRenderer::GetParams()
	{
		ParamValueMap params;

		params.emplace(STRING_TEXTURE, _texture);
		params.emplace(STRING_LAYER, std::to_string(_layer));
		params.emplace(STRING_COLOUR_RED, std::to_string(_colour.x));
		params.emplace(STRING_COLOUR_GREEN, std::to_string(_colour.y));
		params.emplace(STRING_COLOUR_BLUE, std::to_string(_colour.z));
		params.emplace(STRING_COLOUR_ALPHA, std::to_string(_colour.w));
		params.emplace(STRING_FLIPX, std::to_string((int)_flipX));
		params.emplace(STRING_FLIPY, std::to_string((int)_flipY));
		params.emplace(STRING_UI, std::to_string((int)_ui));

		return { STRING_COMPONENT_RENDERER, params };
	}
}

