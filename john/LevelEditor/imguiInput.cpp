/******************************************************************************/
/*!
\file		imguiInput.cpp
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

#include "imguiMgr.h"
#include "StringConverter.h"

namespace FwEngine
{
	void ImguiMgr::ImGUIInput(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		CDEBUG_ERROR_IF(ImGui::GetCurrentContext() == NULL);

		ImGuiIO& io = ImGui::GetIO();
		// Mouse input
		io.MouseDown[0] = (*_input)(IMOUSE_LBUTTON, SystemInput::HOLD);
		io.MouseDown[1] = (*_input)(IMOUSE_RBUTTON, SystemInput::HOLD);

		// Mouse Position
		io.MousePos = { (float)(*_input).GetMousePos().x, (float)(*_input).GetMousePos().y };
		
		// Keyboard Press
		BYTE keyboard[256];
		try
		{
			CDEBUG_ASSERT(GetKeyboardState(keyboard));
		}
		catch (...)
		{
			//CDEBUG_PRINT("Cannot get Keyboard State!");
		}

		// Keys Input
		for (int i = IMOUSE_MBUTTON + 1; i < IKEY_FINAL_VALUE; ++i)
		{
			io.KeysDown[i] = (*_input)(i, SystemInput::HOLD);
			if ((*_input)(i, SystemInput::PRESS))
			{
				if (i == IKEY_ALT)
					io.KeyAlt = (*_input)(i, SystemInput::HOLD);
				else if (i == IKEY_CTRL || i == IKEY_RCTRL || i == IKEY_LCTRL)
					io.KeyCtrl = (*_input)(i, SystemInput::HOLD);
				else if (i == IKEY_SHIFT || i == IKEY_LSHIFT || i == IKEY_RSHIFT)
					io.KeyShift = (*_input)(i, SystemInput::HOLD);
				else if (i == VK_OEM_3)
					_displayImGUI = !_displayImGUI;
			}

			/*
			else
			{
				if ((*_input)(i, SystemInput::PRESS))
				{
					
					ToAscii((UINT)i, MapVirtualKey(i, MAPVK_VK_TO_VSC), keyboard, &keybuff, 0);
					keybuff = (BYTE)keybuff;
					io.AddInputCharacter((unsigned int)keybuff);
					
					// `~ key
					if (VK_OEM_3 == i)
						_displayImGUI = !_displayImGUI;
						
				}
			}
			*/
		}

		io.DeltaTime = dt * 0.0001f;
		
	}
}