/******************************************************************************/
/*!
\file		StringConverter.h
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once

#include <windows.h>
#include <shobjidl.h> 
#include <string>

// Converts Wide String to String
std::string UTF8_encode(const std::wstring& wstr);

// Convert an UTF8 string to a wide Unicode String
std::wstring UTF8_decode(const std::string& str);
