/******************************************************************************/
/*!
\file		main.cpp
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

#include "Framework.h"
#include "Resource.h"
#include "WindowSystem.h"

#include "LEEngine.h"
#include <time.h>
#include "DebugDiagnostic.h"

#include "SystemInput.h"

void MemoryLeakCheck(int breakAlloc = -1)
{
	//Set the leak checking flag
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	//If a valid break alloc provided set the breakAlloc
	if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
}

INT WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInst);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	MemoryLeakCheck();
	
	/*//Create console for debug
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle(TEXT("Debug Mode"));
		//printf("Debug Info\n");
	}*/
	
	//HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_GAM200));

	//IF move into engine must remember to serialize/initialize first before activating
	std::shared_ptr<FwEngine::ResourceManager>resource = std::make_shared<FwEngine::ResourceManager>();
	std::shared_ptr<FwEngine::WindowSystem>windowsMain = nullptr;
	try
	{
		windowsMain = std::make_shared<FwEngine::WindowSystem>();
		windowsMain->ActivateWindow();
	}
	catch (...)
	{
		return 1;
	}

	FwEngine::LEEngine engine;
	engine.Init(windowsMain.get(), &FwEngine::WindowSystem::Update);
	engine.run();

	return 0;
}