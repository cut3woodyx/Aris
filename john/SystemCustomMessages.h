/******************************************************************************/
/*!
\file		SystemCustomMessages.h
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

#ifndef _SYSTEM_CUSTOM_MESSAGE
#define _SYSTEM_CUSTOM_MESSAGE

#include <Windows.h>
#include "SystemIMessage.h"

namespace FwEngine
{
	struct InputMessage : public IMessage
	{
		POINT _cursorPos;
		unsigned int _key;
		InputMessage(POINT pK, unsigned int key) : IMessage("InputKey"), _cursorPos{ pK }, _key{ key } {}
		~InputMessage() = default;
	};
}


#endif