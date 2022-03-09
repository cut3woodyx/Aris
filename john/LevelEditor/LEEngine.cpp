/******************************************************************************/
/*!
\file		LEEngine.cpp
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

#include "LEEngine.h"
#include <time.h>

#include <iostream>

namespace FwEngine
{

	constexpr auto MAX_DT = 16; // in ms

	constexpr auto DT_SCALE = 0.001f; // 1 sec / 1000 to scale dt to ms

	//constexpr auto FPS_60 = 1000.0f / 60.0f;
	constexpr auto FPS_60 = 16.0f; // in ms

	void LEEngine::Init(WindowSystem* window, WinMainUpdate winMain)
	{
		_window = window;
		_winMainUpdate = winMain;
		_sysMgr._windows = window;
		_sysMgr.Init();
		_engineState = ENGINE_UPDATE;

	}

	void LEEngine::run()
	{
		float fixed_dt = FPS_60;
		float dt = 0.0f;
		float accumulatedTime = 0.0f;
		int currentNumberOfSteps = 0;
		clock_t t_start = clock();
		while (_engineState)
		{

			currentNumberOfSteps = 0;

			dt = (float)clock() - t_start;
			t_start = clock();

			//while (dt < fixed_dt)
			//{
			//	dt = (float)clock() - t_start;
			//}

			accumulatedTime += dt;

			while (accumulatedTime >= fixed_dt)
			{
				accumulatedTime -= fixed_dt;
				++currentNumberOfSteps;
			}

			// update system
			if (_sysMgr.Update(fixed_dt * DT_SCALE, currentNumberOfSteps))
			{
				// returns 1
				_engineState = ENGINE_QUIT;
			}

			// win
			//loop = windowsMain->Update(dt);
			WINMAIN_MESSAGE winMsg = (_window->*_winMainUpdate)(dt);
			if (winMsg != WINMAIN_NOMESSAGE)
			{
				if (winMsg == WINMAIN_QUIT)
					_engineState = ENGINE_QUIT;
			}

			// fps
			//float fps = 1000.0f / dt;
			//std::cout << fps << std::endl;
			//std::cout << "60.00" << std::endl; // best solution for 60fps
		}

		// free all memory
		_sysMgr.Free();
	}
}