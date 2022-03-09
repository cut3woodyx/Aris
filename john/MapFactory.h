/******************************************************************************/
/*!
\file		MapFactory.h
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

#include "Map.h"

namespace FwEngine
{

	class MapFactory
	{
	public:
		MapFactory() = default;
		~MapFactory() = default;
		void BuildMap(std::string fileName, std::vector<std::string>& textures, Map* map);
	};

}