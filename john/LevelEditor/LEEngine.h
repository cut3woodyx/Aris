/******************************************************************************/
/*!
\file		LEEngine.h
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
#include "LESysMgr.h"
#include "WindowSystem.h"

namespace FwEngine
{
	typedef WINMAIN_MESSAGE(WindowSystem::* WinMainUpdate)(float);

	enum ENGINESTATE
	{
		ENGINE_QUIT = 0,
		ENGINE_UPDATE,
	};

	class LEEngine
	{
		LESysMgr _sysMgr;
		WindowSystem* _window;
		WinMainUpdate _winMainUpdate;
		ENGINESTATE _engineState;
	public:
		LEEngine() : _window(nullptr), _engineState(ENGINE_UPDATE), _winMainUpdate{ nullptr } {};
		~LEEngine() = default;

		void Init(WindowSystem* window, WinMainUpdate winMain);
		void run();
	};
}
