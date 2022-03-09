/******************************************************************************/
/*!
\file		ComponentUI.h
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
#include "ComponentTransform.h"
#include "Math.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_UI = "ui";
	class SystemGraphics;

	class ComponentUI : public IComponent
	{
	private:
		FwMath::UINT _textureID;
		ComponentTransform* _componentTransform;

	public:
		std::string _texture;
		int _layer;
		FwMath::Vector4D _colour;

		ComponentUI();
		ComponentUI(GameObject* parent);
		~ComponentUI();

		//Use SetTexture to ensure loading of texture as you change textures
		void SetTexture(const std::string& newTexture);
		FwMath::UINT GetTexture();

		ComponentTransform* GetTransform();
		void SetTransform(ComponentTransform* transform);

		void Init(ParamValueMap& paramValues) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}

