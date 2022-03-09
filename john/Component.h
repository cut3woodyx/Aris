/******************************************************************************/
/*!
\file		Component.h
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include <unordered_map>
#include <string>

namespace FwEngine
{
	//constexpr auto OBJ_MAX = 1000;

	constexpr auto OBJ_MAX = 1600;

	//constexpr auto OBJ_MAX = 3200;

	typedef std::unordered_map<std::string, std::string> ParamValueMap;

	/*enum Component
	{
		COMPONENT_ANIMATION,
		COMPONENT_PHYSICS, // rigidbody
		COMPONENT_COLLISION,
		COMPONENT_RENDERER,
		COMPONENT_TRANSFORM,
		COMPONENT_TEXT,
		COMPONENT_AUDIO,

		COMPONENT_SCRIPT,

		COMPONENT_END
	};*/

	// inline ?
	std::string GetFirst(ParamValueMap& paramValues, std::string key);


}
