/******************************************************************************/
/*!
\file		ComponentText.h
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
#include "FWMath.h"
#include <map>
#include "ComponentTransform.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_TEXT = "text";
	struct FontAtlas;

	class ComponentText : public IComponent
	{
	// //Private if not gonna appear on level editor
	private:
		FontAtlas* _charMap;
		ComponentTransform* _componentTransform;

	public:
		FwMath::UINT _pixel;
		FwMath::Vector4D _colour;

		std::string _fontType;
		std::string _displayText;

		ComponentText();
		ComponentText(GameObject* parent);
		~ComponentText();

		void SetFont(const std::string& type, FwMath::UINT pixel);
		FontAtlas* GetFont();

		ComponentTransform* GetTransform();
		void SetTransform(ComponentTransform* transform);

		int CountLines();
		float CalculateRowWidth(std::string::const_iterator it);
		float CalculateMaxRowWidth();

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}
