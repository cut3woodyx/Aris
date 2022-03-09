/******************************************************************************/
/*!
\file		Component.cpp
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
#include "Component.h"

namespace FwEngine
{
	std::string GetFirst(ParamValueMap& paramValues, std::string key)
	{
		auto val = paramValues.find(key);
		if (val == paramValues.end())
			return std::string("0");
		return val->second;
	}
}