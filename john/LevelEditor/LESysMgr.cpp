/******************************************************************************/
/*!
\file		LESysMgr.cpp
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

#include "LESysMgr.h"

#include <iostream>
#include <string>

#include "MapFactory.h"
#include "Map.h"

namespace FwEngine
{
	LESysMgr::LESysMgr() : SystemManager()
	{
	}

	LESysMgr::~LESysMgr()
	{
		Free();
	}

	void LESysMgr::Init()
	{
		SystemManager::Init();

		_imguiMgr.Init(this);

		SystemManager::Update(0, 0, true);


		_systemScript._otherPools.push_back(&_prefabPool); // to init prefap pool

		_systemScript.Init(); // add this to init scr when level is empty

		LoadFabs();
	}

	int LESysMgr::Update(float dt, int steps, bool)
	{
		CameraControl();

		_systemFuncTime.clear();
		LARGE_INTEGER timer;
		// Add new cpu time to get time when function is complete
		auto addToCounter = [&](std::string name)
		{
			QueryPerformanceCounter(&timer);
			_systemFuncTime.emplace_back(name, timer);
		};

		if (!_imguiMgr.GetIfPlayEngine())
		{
			// Start counter
			addToCounter("Start");

			// Input update
			_systemInput.Update(dt);
			addToCounter("Input");

			// Graphics update
			_systemGraphics.Update(dt);
			addToCounter("Graphics");

			// Audio update
			_systemAudio.Update(dt);
			addToCounter("Audio");
		}
		else
		{
			// when play  set ->
			_level = STARTLEVEL_INDEX;
			_levelMax = STARTLEVEL_INDEX;
			SystemManager::Update(dt, steps, true);
		}

		switch (_imguiMgr.Update(dt))
		{
		case 1:
			return 1;
			break;
		}

		::SwapBuffers(_systemGraphics.GetContext()); //using double buffering

		if (_systemInput(IKEY_END, SystemInput::PRESS))
		{
			_systemGraphics._isPrintUI = !_systemGraphics._isPrintUI;
		}

		return 0;
	}

	void LESysMgr::CameraControl()
	{
		if (
			((_systemInput)(IMOUSE_MBUTTON, SystemInput::HOLD) ||
			((_systemInput)(IMOUSE_LBUTTON, SystemInput::HOLD) && (_systemInput)(IKEY_CTRL, SystemInput::HOLD)))
			&& !_imguiMgr._mouseCaptured && !_imguiMgr._mouseUsed)
		{
			POINT vec = _systemInput.GetMouseDragVec();
			_systemGraphics._cameraPos += {(float)vec.x* _systemGraphics._zoom, -(float)vec.y * _systemGraphics._zoom, 0};
			_systemGraphics.UpdateCameraView();
		}
		else
			_systemInput.GetMousePos();
	}
	void LESysMgr::Load(std::string objectFilename, std::string mapFileName, bool disableScriptInit, bool useSavScn)
	{
		SystemManager::Load(objectFilename, mapFileName, disableScriptInit, useSavScn);
	}
	void LESysMgr::Free()
	{
		SystemManager::Free();
	}

	//LESysMgr::LESysMgr() : _currentDt(0), _windows(nullptr), _cursorPosition{ 0 },
	//	_gameObjectPool{}, _gameObjectFactory{}, _systemPhysics{}, _systemGraphics{},
	//	_systemScript{}, _systemInput{}, _map{}
	//{
	//}


	//LESysMgr::~LESysMgr()
	//{
	//	Free();
	//}

	//void LESysMgr::Init()
	//{
	//	// load from serialisatiom
	//	// load textures, meshes etc.
	//	_gameObjectFactory.SetCurrentPool(&_gameObjectPool);

	//	_systemGraphics.SetGameObjectPool(&_gameObjectPool);
	//	_systemGraphics.SetWindowProperties(_windows->GetWinHandle(), _windows->_resolutionWidth, _windows->_resolutionHeight);

	//	_gameObjectFactory.SetSystemGraphics(&_systemGraphics);

	//	// SET INPUT
	//	_systemInput.Init();
	//	_systemInput.SetGameObjectPool(&_gameObjectPool);
	//	_systemInput.SetHandle(_windows->GetWinHandle());

	//	//Set Input into windows system
	//	_windows->SetInputSystem(&_systemInput);

	//	// must after createSerialisedGameObjects init // need to do render sort to add or wun render
	//	_systemGraphics.Init();

	//	_systemGraphics._map = &_map;

	//	//Load("Resources/Objects/Test.txt", "Resources/Map/Test_Map.csv");
	//	//Free();

	//	_imguiMgr.Init(this);

	//	_systemPhysics.SetGameObjectPool(&_gameObjectPool);
	//	_systemPhysics.SetMap(&_map);

	//	_systemScript._systemManager = (SystemManager*)this;
	//	_systemScript.SetGameObjectPool(&_gameObjectPool);
	//	_systemScript.Init();
	//}

	//int LESysMgr::Update(float dt, int steps)
	//{
	//	_systemInput.Update(dt);
	//	for (int step = 0; step < steps; ++step)
	//	{
	//		CursorPos pos =
	//			_systemInput.GetMouseWorldPos(_systemGraphics._cameraPos.x,
	//				_systemGraphics._cameraPos.y, _systemGraphics.GetHeight(), _systemGraphics.GetWidth(), _systemGraphics._zoom);
	//		_cursorPosition = FwMath::Vector3D(pos.x, pos.y, 0);

	//		if (_imguiMgr.GetIfPlayEngine())
	//		{
	//			_systemScript.Update(dt);
	//			_systemPhysics.Update(dt);
	//		}

	//		CameraControl();
	//	}	

	//	_systemGraphics.Update(dt);
	//	switch (_imguiMgr.Update(dt))
	//	{
	//	case 1:
	//		return 1;
	//		break;
	//	}

	//	::SwapBuffers(_systemGraphics.GetContext()); //using double buffering

	//	return 0;
	//}

	//void LESysMgr::CameraControl()
	//{
	//	if ((_systemInput)(IMOUSE_LBUTTON, SystemInput::HOLD) && !_imguiMgr._mouseCaptured && !_imguiMgr._mouseUsed)
	//	{
	//		POINT vec = _systemInput.GetMouseDragVec();
	//		_systemGraphics._cameraPos += {(float)vec.x * _systemGraphics._zoom, -(float)vec.y * _systemGraphics._zoom, 0};
	//		_systemGraphics.UpdateCameraView();
	//	}
	//	else
	//		_systemInput.GetMousePos();
	//}
	//void LESysMgr::Load(std::string objectFilename, std::string mapFileName)
	//{
	//	// cerealisation
	//	if (!objectFilename.empty())
	//		_gameObjectFactory.CreateSerialisedGameObjects(objectFilename.c_str());

	//	// load map from cerealisztion
	//	// must init gfx first
	//	if (!mapFileName.empty())
	//	{
	//		_map.Init(mapFileName.c_str());
	//		_systemGraphics._map = &_map;
	//	}

	//	_systemScript.Init();
	//	_systemScript.InitAllScripts();
	//}
	//void LESysMgr::Free()
	//{
	//	_gameObjectPool.Free();
	//	_map.Free();
	//	_systemScript.Free();
	//}
}

