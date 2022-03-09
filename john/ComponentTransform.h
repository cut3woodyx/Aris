/******************************************************************************/
/*!
\file		ComponentTransform.h
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
#pragma once

#include "IComponent.h"
#include "FWMath.h"
#include <string>

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_TRANSFORM = "transform";
	class ComponentTransform : public IComponent
	{
	public:

		std::string _tag;

		FwMath::Vector3D _currentPosition; // translation
		FwMath::Vector3D _scale;
		float _rotation;

		ComponentTransform();
		ComponentTransform(GameObject* parent);
		~ComponentTransform();

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}

