/******************************************************************************/
/*!
\file		ComponentRenderer.h
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
#include <string>
#include "ShaderSystem.h"
#include "GraphicENUM.h"
#include "ComponentTransform.h"
#include "ComponentAnimation.h"
#include "ComponentEmitter.h"
#include "Math.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_RENDERER = "renderer";
	class SystemGraphics;

	class ComponentRenderer : public IComponent
	{
	private:
		FwMath::UINT _textureID;

		ShaderType _type;

		ComponentTransform* _componentTransform;
		ComponentAnimation* _componentAnimation;
		ComponentEmitter* _componentEmitter;

	public:
		std::string _texture;
		int _layer;
		FwMath::Vector4D _colour;
		bool _flipX;
		bool _flipY;
		bool _ui;
	
		ComponentRenderer();
		ComponentRenderer(GameObject* parent);
		~ComponentRenderer();

		//Use SetTexture to ensure loading of texture as you change textures
		void SetTexture(const std::string& newTexture);
		FwMath::UINT GetTexture();

		ComponentTransform* GetTransform();
		ComponentAnimation* GetAnimation();
		ComponentEmitter* GetEmitter();

		void SetTransform(ComponentTransform* transform);
		void SetAnimation(ComponentAnimation* animation);
		void SetEmitter(ComponentEmitter* emitter);

		ShaderType GetShaderType();
		void SetShaderType(ShaderType type);

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}

