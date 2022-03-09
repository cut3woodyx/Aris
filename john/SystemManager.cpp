/******************************************************************************/
/*!
\file		SystemManager.cpp
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
#include <iostream>
#include "SystemManager.h"
#include <string>

#include "MapFactory.h"
#include "Map.h"

#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <filesystem>

//#include <pthread.h> 

constexpr auto PATH_MENU = "Resources/Objects/MENU.txt";

namespace FwEngine
{
	void SystemManager::PrintObjects()
	{
		// test
	}

	void SystemManager::LoadLevelConfig()
	{

		menu_objects = std::string(PATH_MENU);
		menu_map = std::string("Resources/Map/Empty_Map.csv");

		filepath_objects.push_back(std::string("Resources/Objects/ANOTHERMENU.txt"));
		filepath_objects.push_back(menu_objects);

		filepath_map.push_back(std::string("Resources/Map/Empty_Map.csv"));
		filepath_map.push_back(menu_map);

		sceneNames.push_back("menu");
		sceneNames.push_back("anothermenu");

		//filepath_objects.push_back(std::string("Resources/Objects/ANOTHERMENU.txt"));
		//filepath_objects.push_back(std::string("Resources/Objects/MENU.txt"));
		////filepath_objects.push_back(std::string("Resources/Objects/Test.txt"));

		//filepath_map.push_back(std::string("Resources/Map/Empty_Map.csv"));
		//filepath_map.push_back(std::string("Resources/Map/Empty_Map.csv"));
		////filepath_map.push_back(std::string("Resources/Map/Test_Map.csv"));

		// read from level config file
		TextSerializer readFromFile;
		CDEBUG_ASSERT(readFromFile.OpenRead("Resources/Configuration/LevelConfig.txt"), "Unable to Open File!");
		std::string readStr;

		while (true)
		{
			readFromFile.ReadStringLine(readStr);
			if (readStr._Equal("END"))
				break;
			sceneNames.push_back(readStr);
			//readStr.empty();

			readFromFile.ReadStringLine(readStr);
			if (readStr._Equal("END"))
			{
				CDEBUG_ASSERT(nullptr, "LevelConfig File Error!");
			}
			filepath_objects.push_back(readStr);
			//readStr.empty();

			readFromFile.ReadStringLine(readStr);
			if (readStr._Equal("END"))
			{
				CDEBUG_ASSERT(nullptr, "LevelConfig File Error!");
			}
			filepath_map.push_back(readStr);
			//readStr.empty();
		}

		_levelMax = (int)filepath_objects.size() - 1;
	}
	
	void SystemManager::SplashInit()
	{
		const std::string PATH_GGPENLOGO = ("Resources/Sprites/splash_digipenlogo.png");
		const std::string PATH_TEAMLOGO  = ("Resources/Sprites/splash_teamlogo.png");
		const std::string PATH_TEAMLOGO_BGM = ("Resources/Audio/Logo.ogg");

		_splashState = 0;

		//
		GameObject* obj1 = _gameObjectFactory.CreateGameObject_Empty(
			FwMath::Vector3D(1920.0f, 1080.0f, 0.0f)
		);
		ComponentRenderer* compRender1 = _gameObjectFactory.GameObject_AttachComponent_Renderer(
			obj1->GetIndex(),
			PATH_GGPENLOGO
		);
		_objpVec.push_back(obj1);
		compRender1->disable();
		//

		//
		GameObject* obj2 = _gameObjectFactory.CreateGameObject_Empty(
			FwMath::Vector3D(1920.0f, 1080.0f, 0.0f)
		);
		ComponentRenderer* compRender2 = _gameObjectFactory.GameObject_AttachComponent_Renderer(
			obj2->GetIndex(),
			PATH_TEAMLOGO
		);

		ComponentAudio* compAudio2 = _gameObjectPool.GetComponent<ComponentAudio>(obj2->GetIndex());
		compAudio2->Free();
		compAudio2->AddNewAudio();
		compAudio2->_audioNames[0] = "Logo";
		compAudio2->_audioFileNames[0] = PATH_TEAMLOGO_BGM;
		compAudio2->enable();

		_objpVec.push_back(obj2);
		compRender2->disable();
		//
	}

	void SystemManager::SplashUpdate(float dt)
	{
		if (_objpVec.size() < _splashState)
		{
			throw;
		}
		if (_splashState != 0)
		{
			size_t ind = ((size_t)_splashState - 1);
			GameObject* obj = _objpVec[ind];
			ComponentRenderer* compRender = _gameObjectPool.GetComponent<ComponentRenderer>(obj->GetIndex());
			compRender->disable();
		}
		GameObject* obj = _objpVec[_splashState];
		ComponentRenderer* compRender = _gameObjectPool.GetComponent<ComponentRenderer>(obj->GetIndex());
		compRender->enable();

		_systemGraphics.Update(dt);

		if (_splashState == 1)
		{
			ComponentAudio* audio = _gameObjectPool.GetComponent<ComponentAudio>(obj->GetIndex());
			audio->Play("Logo",false);
			while (audio->IsPlaying())
			{
				_systemAudio.Update(dt);
			}
		}

		_splashState++;
	}

	void SystemManager::SplashEnd()
	{
		if (_splashState == 0)
			return;
		for (GameObject* obj : _objpVec)
		{
			obj->disable();
			ComponentRenderer* compRender = _gameObjectPool.GetComponent<ComponentRenderer>(obj->GetIndex());
			ComponentAudio* audio = _gameObjectPool.GetComponent<ComponentAudio>(obj->GetIndex());
			audio->Free();
			_systemGraphics.DeleteRenderer(compRender);
		}
		_objpVec.clear();
	}


	void SystemManager::FakeLoadingScrInit()
	{
		const std::string PATH_LOADINGSPLASH = ("Resources/UI/Loading.jpg");

		//
		_splashObj = _gameObjectFactory.CreateGameObject_Empty(
			//FwMath::Vector3D(1920.0f, 1080.0f, 0.0f),
			//FwMath::Vector3D(1600.0f, 900.0f, 0.0f),
			FwMath::Vector3D((float)_systemGraphics.GetWidth(), (float)_systemGraphics.GetHeight(), 0.0f)
			//_systemGraphics._cameraPos
		);
		_splashRender = _gameObjectFactory.GameObject_AttachComponent_Renderer(
			_splashObj->GetIndex(),
			PATH_LOADINGSPLASH,
			100
		);

		//_splashRender->_flipX = false;
		//_splashRender->_flipY = false;

		//_splashRender->_ui = true; ?
		//
		_systemGraphics._isPrintUI = false;

		_systemGraphics._cameraPos = FwMath::Vector3D(0.0f, 0.0f, 0.0f);
		_systemGraphics._zoom = 1.0f;
		_systemGraphics.UpdateZoom();
		_systemGraphics.UpdateCameraView();
	}

	void SystemManager::FakeLoadingScrEnd()
	{
		_splashObj->disable();
		_splashRender->disable();
		_systemGraphics._isPrintUI = true;
	}

	void SystemManager::LoadingInit()
	{
		Free();


		LoadFabs();



		_systemGraphics._cameraPos = FwMath::Vector3D(0.0f, 0.0f, 0.0f);
		_systemGraphics._zoom = 1.0f;
		_systemGraphics.UpdateZoom();
		_systemGraphics.UpdateCameraView();


		if (filepath_objects.size() != 0 || filepath_map.size() != 0)
		{
			Load(filepath_objects[_level], filepath_map[_level], false, true);

			//_prevScene = _currentScene;
			//_currentScene = sceneNames[_level];

		}
	}

	void SystemManager::LoadingEnd()
	{
		_gamestateNext = GAMESTATE_UPDATE;
	}

	void SystemManager::LoadingUpdate(float)
	{


		LoadingEnd(); // 
	}

	SystemManager::SystemManager() : 
		_currentDt(0), 
		_windows(nullptr), 
		_gamestate(GAMESTATE_LOAD_MAINMENU /*GAMESTATE_LOAD*//*GAMESTATE_UPDATE*/),
		_gamestateNext(GAMESTATE_LOAD_MAINMENU /*GAMESTATE_LOAD*//*GAMESTATE_UPDATE*/),
		_menustate(MENUSTATE_MAIN),
		_menustateNext(MENUSTATE_MAIN),
		_cursorPosition{ 0, 0, 0 }, 
		_isPhysicsRunning(true), 
		_isScriptRunning(true),
		_level(MAINMENU_INDEX),
		_levelMax(0),
		_currentFPS(0.0f),
		_currentObj(0),
		_isDebug(false),
		_isTest(false),
		_testCount(0),
		_playerMgr(),
		_loadingGameObjectPool(LOAD_SIZE),
		_prefabPool(FAB_SIZE),
		_isFabsLoaded(false),
		_splashObj(nullptr),
		_splashRender(nullptr)
	{
		//LoadLevelConfig();

		_loadingGameObjectFactory.SetCurrentPool(&_loadingGameObjectPool);
		_prefabFac3.SetCurrentPool(&_prefabPool);
	}


	SystemManager::~SystemManager()
	{
		//std::map<std::string, GameObjectPool>::iterator itr = _objectPoolMap.begin();
		//while (itr != _objectPoolMap.end())
		//{
		//	(itr->second).Free();
		//	++itr;
		//}
		//_objectPoolMap.clear();
		_gameObjectPool.Free();
	}

	void SystemManager::Init()
	{
		//Seed the pseudo random number generator
		srand(static_cast <unsigned> (time(0)));

		// load from serialisatiom
		// load textures, meshes etc.
		_gameObjectFactory.SetCurrentPool(&_gameObjectPool);
		
		_systemPhysics.SetGameObjectPool(&_gameObjectPool);
		_systemPhysics.SetMap(&_map);
		_systemGraphics.SetGameObjectPool(&_gameObjectPool);
		_systemGraphics.SetGraphicsProperties(_windows->GetWinHandle(), _windows->_resolutionWidth, _windows->_resolutionHeight);
		_systemAudio.SetGameObjectPool(&_gameObjectPool);
		
		_gameObjectFactory.SetSystemGraphics(&_systemGraphics);

		// SET INPUT
		//_systemInput.Init();
		_systemInput.SetGameObjectPool(&_gameObjectPool);
		_systemInput.SetHandle(_windows->GetWinHandle());

		//Set Input into windows system
		_windows->SetInputSystem(&_systemInput);

		// SET EVENT MESSAGING
		/*
		_gameObjectFactory._gameObjListener.AttachHandler("InputKey", &GameObjectFactory::CreateObject, &_gameObjectFactory);
		_systemInput.Register("InputKey", _gameObjectFactory._gameObjListener);
		_systemPhysics._listener.AttachHandler("InputKey", &SystemPhysics::runCol, &_systemPhysics);
		_systemInput.Register("InputKey", _systemPhysics._listener);
		*/
		// cerealisation
		// Test.txt
		//_gameObjectFactory.CreateSerialisedGameObjects("Resources/Objects/Test.txt");

		// must after createSerialisedGameObjects init // need to do render sort to add or wun render
		// must be before cerialisation now bcos it adds to renderer, or will have double render
				// init sys script
		_systemScript._systemManager = this;
		_systemScript.SetGameObjectPool(&_gameObjectPool);

		try
		{
			_systemGraphics.Init();
			_systemAudio.Init();
			SetMasterVolume(_systemAudio.GetSystem(), _windows->_masterVolume);
		}
		catch (ShaderSystem::ShaderException& err)
		{
			(void)err;
			_gamestateNext = GAMESTATE_QUIT;
		}
		catch (...)
		{
			_gamestateNext = GAMESTATE_QUIT;
		}

		// load map from cerealisztion
		// must init gfx first
		//_map.Init("Resources/Map/Test_Map.csv");
		//_map.InitTex(&_systemGraphics);
		//_systemGraphics._map = &_map;


		//_systemScript.functionGetComponent = &_gameObjectPool.GetComponent;
		//_systemScript._systemManager = this;
		//_systemScript.SetGameObjectPool(&_gameObjectPool);
		//_systemScript.Init();

		//for (int i = 0; i < 300; ++i)
		//{
		//	gameObjectFactory.createSerialisedGameObjects("Test.txt");
		//}

		// test
		//PrintObjects();

	}

	GameObject* SystemManager::CreatePrefabObject(std::string name)
	{
		// TODO

		int id = -1;
		for (auto names : _prefabNames)
		{
			if (names.second._Equal(name))
			{
				id = names.first;
				break;
			}
		}
		
		try
		{
			CDEBUG_ASSERT(id != -1, std::string("Fab ").append(name).append(" not found!").c_str());
		}
		catch (...)
		{
			return nullptr;
		}

		GameObject* newGameObj = _gameObjectPool.CloneGameObject(_prefabPool, id);

		if (newGameObj == nullptr)
			return newGameObj;

		ComponentRenderer* compRender = _gameObjectPool.GetComponent<ComponentRenderer>(newGameObj->GetIndex());

		ComponentText* compText = _gameObjectPool.GetComponent<ComponentText>(newGameObj->GetIndex());


		if (compRender->isEnabled())
		{
			_systemGraphics.AddRenderer(static_cast<ComponentRenderer*>(compRender));
		}
		if (compText->isEnabled())
		{
			_systemGraphics.AddText(static_cast<ComponentText*>(compText));
		}

		return newGameObj;
	}

	void SystemManager::LoadFabs()
	{
		// alr loaded, dun do anything
		if (_isFabsLoaded)
			return;

		// Create if doesn't exist
		CreateDirectory(L".\\Resources\\Prefabs", NULL);

		std::string fapath = "Resources/Prefabs";
		for (const auto& entry : std::filesystem::directory_iterator(fapath))
		{
			std::cout << entry.path() << std::endl;

			std::string ext(entry.path().extension().string());

			if (ext._Equal(".efab"))
			{
				std::string filename(entry.path().string());
				GameObject* go = _prefabFac3.CreateSingleSerializedGameObject(filename, true);

				if (go == nullptr)
				{
					//handle
					throw;
				}

				std::string name(entry.path().stem().string());
				_prefabNames.try_emplace((int)go->GetIndex(), name);
			}

		}

		_isFabsLoaded = true;
	}

	void SystemManager::BuildFabs()
	{

	}

	void SystemManager::Load(std::string objectFilename, std::string mapFileName, bool disableScriptInit, bool useSavScn)
	{

		FakeLoadingScrInit();
		_systemGraphics.Update(0.016f);
		FakeLoadingScrEnd();

		if (_gamestate == GAMESTATE_LOAD_RELOAD)
		{
			//std::map<std::string, GameObjectPool>::iterator itr = _objectPoolMap.find(objectFilename);
			//if (itr != _objectPoolMap.end())
			//	(itr->second).Free();
			//_objectPoolMap.erase(itr);

			std::list<std::string>::iterator itr = _objectPoolNames.begin();
			while (itr != _objectPoolNames.end())
			{
				if ((*itr)._Equal(objectFilename))
				{
					_objectPoolNames.erase(itr);
					break;
				}
				++itr;
			}
		}

		//std::map<std::string, GameObjectPool>::iterator itr = _objectPoolMap.find(objectFilename);

		bool hasSavedObj = false;
		std::string saveFileName;
		if (objectFilename._Equal(PATH_MENU))
		{
			hasSavedObj = false;
			saveFileName = PATH_MENU;
			_objectPoolNames.clear();
		}
		else
		{
			std::list<std::string>::iterator itr = _objectPoolNames.begin();
			while (itr != _objectPoolNames.end())
			{

				if ((*itr)._Equal(objectFilename))
				{
					hasSavedObj = true;
					saveFileName = *itr;
					break;
				}
				++itr;
			}
		}

		bool isNewLoad = false;

		// Create if doesn't exist
		if(useSavScn)
			CreateDirectory(L".\\Resources\\Temp", NULL);

		//if (!hasSavedObj && useSavScn)
		//{
		//	// save to file

		//	std::string& cereals = _gameObjectFactory.CerealiseGameObjects();

		//	std::string sapath = "Resources/Temp/";

		//	std::filesystem::directory_entry saentry(_currentObjPath);
		//	std::string safname = saentry.path().filename().string();
		//	if (safname._Equal(""))
		//		safname = "empty.txt";
		//	sapath.append(safname);

		//	TextSerializer txtSer(sapath);
		//	txtSer.Write(cereals);
		//	txtSer.OpenWrite(sapath);

		//	_objectPoolNames.push_back(objectFilename);
		//}

		if (hasSavedObj && useSavScn)
		{
			// load from to file

			_systemScript._otherPools.push_back(&_prefabPool);
			_systemScript._otherPools.push_back(&_loadingGameObjectPool);

			_systemScript.Init();

			std::string sapath = "Resources/Temp/";
			std::filesystem::directory_entry lentry(saveFileName);
			std::string lfname = lentry.path().filename().string();
			sapath.append(lfname);

			_gameObjectFactory.CreateSerialisedGameObjects(sapath);

			//std::vector<ComponentRenderer>* con = _gameObjectPool.GetContainerPtr<ComponentRenderer>();
			//for (ComponentRenderer& ren : *con)
			//{
			//	_systemGraphics.AddRenderer(&ren);
			//}

		}
		else
		{
			
			// this wasted 2h of my time ... need a better way to manage systems in the future.

			_systemScript._otherPools.push_back(&_prefabPool);
			_systemScript._otherPools.push_back(&_loadingGameObjectPool);
			
			_systemScript.Init();

			// //

			//if (objectFilename._Equal(""))
				//return;

			// cerealisation
			if (!objectFilename.empty())
			{
				try
				{
					_gameObjectFactory.CreateSerialisedGameObjects(objectFilename);
					//_gameObjectFactory.CreateSerialisedGameObjects("Resources/Objects/OPTIONS.txt");
					//if (_level >= STARTLEVEL_INDEX)
					//{
					//	_gameObjectFactory.CreateSerialisedGameObjects("Resources/Objects/PAUSE.txt");
					//}
				}
				catch (...)
				{
				//catch (const std::exception & exc)
				//{
				//	std::cout << exc.what();

					_gamestateNext = GAMESTATE_QUIT;
					// return;
				}
			}

			isNewLoad = true;

			//if (!disableScriptInit) _systemScript.InitAllScripts();
		}






		_currentObjPath = objectFilename;
		_currentMapPath = mapFileName;

		// load map from cerealisztion
		// must init gfx first
		//if (mapFileName._Equal(""))
			//return;
		_map.Init(mapFileName);
		_systemGraphics._map = &_map;

		//_systemScript.functionGetComponent = &_gameObjectPool.GetComponent;
		//_systemScript._systemManager = this;
		//_systemScript.SetGameObjectPool(&_gameObjectPool);

		//if(!disableScriptInit && isNewLoad) _systemScript.InitAllScripts();



		_prevScene = _currentScene;
		if (sceneNames.size())
			_currentScene = sceneNames[_level];

		if (!disableScriptInit /*&& isNewLoad*/) _systemScript.InitAllScripts();

		if(!isNewLoad || _gamestate == GAMESTATE_LOAD_RELOAD)
			_systemScript.OnEnterScene();

		//LoadingEnd();
	}

	void SystemManager::Free()
	{
		//GameObjectPool newPool;
		//_gameObjectPool.Swap(newPool);
		////std::swap(_gameObjectPool, newPool);
		//_objectPoolMap.try_emplace(_currentObjPath, newPool);

		//_objectPoolNames.clear();

		// Create if doesn't exist
		//if (useSavScn)
		//	CreateDirectory(L".\\Resources\\Temp", NULL);


		_systemScript.Unload();

		//if (useSavScn)
			// save to file

		std::string& cereals = _gameObjectFactory.CerealiseGameObjects();

		std::string sapath = "Resources/Temp/";

		std::filesystem::directory_entry saentry(_currentObjPath);
		std::string safname = saentry.path().filename().string();
		if (safname._Equal(""))
			safname = "empty.txt";
		sapath.append(safname);

		// doesnt work
		//TextSerializer txtSer(sapath);
		//txtSer.Write(cereals);
		//txtSer.OpenWrite(sapath);

		std::ofstream outfile;
		outfile.open(sapath, std::ios::out | std::ios::trunc);
		outfile << cereals << std::endl;
		outfile.close();

		_objectPoolNames.push_back(_currentObjPath);
		// //


		_systemScript.Free();
		_gameObjectPool.Free();
		_systemPhysics.Free();
		_systemGraphics.Free();
		_systemAudio.Free();
		_systemInput.Free();

		_map.Free();
	}

	int SystemManager::Update(float dt, int steps, bool disableDebugInputs)
	{
		_gamestate = _gamestateNext;
		_menustate = _menustateNext;

		if (_level > _levelMax || _level < 0)
			_level = MAINMENU_INDEX;
		switch (_gamestate)
		{
		case GAMESTATE_LOAD_MAINMENU:
			_gamestateNext = GAMESTATE_UPDATE;
			_level = MAINMENU_INDEX;
			Free();
#ifndef _LEVEL_EDITOR
			SplashInit();

			Sleep(2000);

			SplashUpdate(dt);

			Sleep(2000);

			SplashUpdate(dt);

			SplashEnd();
#endif

			if (filepath_objects.size() != 0 || filepath_map.size() != 0)
			{
				Load(filepath_objects[MAINMENU_INDEX], filepath_map[MAINMENU_INDEX], false, true);
				//Load(menu_objects, menu_map, false, true);
			}

			break;

		//case GAMESTATE_LOAD_ANOTHERMENU:
		//	_gamestateNext = GAMESTATE_UPDATE;
		//	_level = ANOTHERMENU_INDEX;
		//	Free();
		//	if (filepath_objects.size() != 0 || filepath_map.size() != 0)
		//	{
		//		Load(filepath_objects[ANOTHERMENU_INDEX], filepath_map[ANOTHERMENU_INDEX]);
		//	}
		//	break;

		case GAMESTATE_LOAD_LEVEL:

			_systemScript.Update(dt);
			_systemGraphics.Update(dt);

			_gamestateNext = GAMESTATE_UPDATE;
			_level = STARTLEVEL_INDEX; // 

			LoadingInit();

			//_systemScript.OnEnterScene();

			break;

		case GAMESTATE_LOAD_NEXTLEVEL:
			_gamestateNext = GAMESTATE_LOAD;
			_level++; // 
			if (_level > _levelMax || _level < 0)
				_level = MAINMENU_INDEX;

			LoadingInit();

			break;

		case GAMESTATE_LOAD:
			LoadingUpdate(dt);
			break;

		case GAMESTATE_LOAD_RETURNTOMENU:

			_gamestateNext = GAMESTATE_LOAD;

			// fls
			//FakeLoadingScrInit();
			//_systemGraphics.Update(dt);
			//FakeLoadingScrEnd();

			_nextScene = std::string("menu");
			_level = MAINMENU_INDEX; // 

			{
				//std::map<std::string, GameObjectPool>::iterator itr = _objectPoolMap.begin();

				//
				//while (itr != _objectPoolMap.end())
				//{
				//	itr->second.Free();
				//	++itr;
				//}

				////_objectPoolMap.clear();

				//while (!_objectPoolMap.empty())
				//	_objectPoolMap.erase(_objectPoolMap.begin());

				_objectPoolNames.clear();

			}

			//LoadingInit();

			//Free();
			_systemScript.Free();
			_gameObjectPool.Free();
			_systemPhysics.Free();
			_systemGraphics.Free();
			_systemAudio.Free();
			_systemInput.Free();
			_map.Free();
			
			_systemGraphics._cameraPos = FwMath::Vector3D(0.0f, 0.0f, 0.0f);
			_systemGraphics._zoom = 1.0f;
			_systemGraphics.UpdateZoom();
			_systemGraphics.UpdateCameraView();


			if (filepath_objects.size() != 0 || filepath_map.size() != 0)
			{

				Load(filepath_objects[_level], filepath_map[_level], false, true);

				//_prevScene = _currentScene;
				//_currentScene = sceneNames[_level];

			}
			// else?

			/*_systemScript.OnEnterScene();*/
			_playerMgr.reset();


			break;

		case GAMESTATE_LOAD_NEXTSCENE:
			_gamestateNext = GAMESTATE_LOAD;
			// Load scene based on stored str
			//_level++; 

			//_systemScript.Update(dt);
			//FakeLoadingScrInit();
			//_systemGraphics.Update(dt);
			//FakeLoadingScrEnd();

			// get level
			if (_nextScene._Equal("menu")) // Clear
			{
				_level = MAINMENU_INDEX;

				// reset event keys

				//_playerMgr.keys = 0;
				//_playerMgr.keyLocation.clear();
				//_playerMgr.isRestrickMovement = false;
				//_playerMgr.isShakeEvent = false;

				_playerMgr.reset();
			}
			else
			{
				int index = -1;
				for (std::string& scene : sceneNames)
				{
					++index;
					if (scene._Equal(_nextScene))
					{
						break;
					}
				}
				_level = index;
			}
			// error
			if (_level > _levelMax || _level < 0)
				_level = MAINMENU_INDEX;

			LoadingInit();

			//_systemScript.OnEnterScene();

			break;

		case GAMESTATE_LOAD_RELOAD:
			_gamestateNext = GAMESTATE_LOAD;

			//FakeLoadingScrInit();
			//_systemGraphics.Update(dt);
			//FakeLoadingScrEnd();

			Free();

			LoadFabs();

			_systemGraphics._cameraPos = FwMath::Vector3D(0.0f, 0.0f, 0.0f);
			_systemGraphics._zoom = 1.0f;
			_systemGraphics.UpdateZoom();
			_systemGraphics.UpdateCameraView();

			Load(filepath_objects[_level], filepath_map[_level], false, true);

			//LoadingInit();

			//_systemScript.OnEnterScene();

			break;

		case GAMESTATE_QUIT:
			Free();
			return 1;
			break;
		}

		_systemFuncTime.clear();
		LARGE_INTEGER timer;
		auto addToCounter = [&](std::string name)
		{
			QueryPerformanceCounter(&timer);
			_systemFuncTime.emplace_back(name, timer);
		};
		addToCounter("Start");
		
		if (_gamestate == GAMESTATE_UPDATE || _gamestate == GAMESTATE_UPDATE_STEP)
		{
			CursorPos pos =
				_systemInput.GetMouseWorldPos(_systemGraphics._cameraPos.x,
					_systemGraphics._cameraPos.y, _systemGraphics.GetHeight(), _systemGraphics.GetWidth(), GRAPHICS->_zoom  /*_systemGraphics._zoom*/);
			_cursorPosition = FwMath::Vector3D(pos.x, pos.y, 0);


			if (_gamestate == GAMESTATE_UPDATE_STEP)
				steps = 1;

			// limit max steps to 5
			if (steps > 5)
				steps = 5;

			// .. TODO
			for (int step = 0; step < steps; ++step)
			{
				_systemFuncTime.clear();
				addToCounter("Start");


				if (_isScriptRunning)
				{
					_systemScript.Update(dt);
					addToCounter("Script");
				}

				if (_isPhysicsRunning)
				{
					_systemPhysics.Update(dt);
					addToCounter("Physics");
				}

				_systemInput.Update(dt); // optimise
				QueryPerformanceCounter(&timer);
				addToCounter("Input");
			}

			_systemAudio.Update(dt);
			addToCounter("Audio");
			//do last
			//systemAnimation.update(currentDt);`
			_systemGraphics.Update(dt);
			addToCounter("Graphics");


			if (_systemInput(IKEY_ESCAPE, SystemInput::PRESS) /*&& !disableDebugInputs*/)
			{
				_gamestateNext = GAMESTATE_PAUSE;
			}
			if (_gamestate == GAMESTATE_UPDATE_STEP)
			{
				_gamestateNext = GAMESTATE_PAUSE_STEP;
			}

			//if (_systemInput(IKEY_N, SystemInput::PRESS) /*&& !disableDebugInputs*/)
			//{
			//	_gamestateNext = GAMESTATE_LOAD_NEXTLEVEL;
			//}
		}
		else if (_gamestate == GAMESTATE_PAUSE || _gamestate == GAMESTATE_PAUSE_STEP)
		{
			// pause 
			if (_isScriptRunning)
			{
				_systemScript.Update(dt);
				addToCounter("Script");
			}

			_systemInput.Update(dt);
			addToCounter("Input");

			_systemGraphics.Update(dt);
			addToCounter("Graphics");

			if (_systemInput(IKEY_ESCAPE, SystemInput::PRESS) /*&& !disableDebugInputs*/)
			{
				_gamestateNext = GAMESTATE_UPDATE;
			}
			/*if (_systemInput(IKEY_O, SystemInput::PRESS) /*&& !disableDebugInputs*///)
			/*{
				_gamestateNext = GAMESTATE_UPDATE_STEP;
			}*/
		}

		// disable systems
		if (_systemInput(IKEY_I, SystemInput::PRESS) && !disableDebugInputs)
		{
			_isPhysicsRunning = !_isPhysicsRunning;
		}
		if (_systemInput(IKEY_U, SystemInput::PRESS) && !disableDebugInputs)
		{
			_isScriptRunning = !_isScriptRunning;
		}
		if (_systemInput(IKEY_Y, SystemInput::PRESS) && !disableDebugInputs)
		{
			_systemPhysics._runCollision = !_systemPhysics._runCollision;
		}

		// gamestates unload and quit
		if (_systemInput(IKEY_BACK, SystemInput::PRESS) && !disableDebugInputs)
		{
			_gamestateNext = GAMESTATE_LOAD_RELOAD;
		}
		//if (_systemInput(IKEY_ESCAPE, SystemInput::PRESS) && !disableDebugInputs)
		//{
			//_gamestateNext = GAMESTATE_QUIT;
		//}

		//debug
		if (_systemInput(IKEY_9, SystemInput::PRESS) && !disableDebugInputs)
		{
			_isDebug = !_isDebug;
		}

		if (_isDebug)
		{
			// get no of game objs
			_currentObj = 0;
			GameObjectPool::ContainerType_GameObject* goc = _gameObjectPool.GetContainerPtr<GameObject>();
			for (GameObject& obj : *goc)
			{
				if (obj._isActive)
					++_currentObj;
			}
			//std::cout << " No of Obj : "<< num << " FPS : ";

		}

		/*if (_systemInput(IKEY_0, SystemInput::PRESS) && !disableDebugInputs)
		{
			_isTest = !_isTest;
			_testCount = 0;
		}
		if (_isTest && _testCount < 1500)
		{
			_testCount++;
			_gameObjectFactory.CreateGameObject_Test1(_cursorPosition, FwMath::Vector3D((float)(rand() % 200 - 100), (float)(rand() % 100 + 1), 0.0f));

			_testCount++;
			_gameObjectFactory.CreateGameObject_Test1(_cursorPosition, FwMath::Vector3D((float)(rand() % 200 - 100), (float)(rand() % 100 + 1), 0.0f));

			_testCount++;
			_gameObjectFactory.CreateGameObject_Test1(_cursorPosition, FwMath::Vector3D((float)(rand() % 200 - 100), (float)(rand() % 100 + 1), 0.0f));

		}*/
		// return no events
		return 0;
	}


	ComponentText* SystemManager::AddTextObject()
	{
		GameObject* newTextObj = _gameObjectFactory.CreateGameObject_Empty
		(
			FwMath::Vector3D(1.0f, 1.0f, 1.0f),
			FwMath::Vector3D(0.0f, 0.0f, 0.0f)
		);
		int id = newTextObj->GetIndex();
		ComponentText* compText = _gameObjectFactory.GameObject_AttachComponent_Text(id, "");
		_systemGraphics.AddText(compText);
		return compText;
	}

	void SystemManager::PlayerManager::addKeyLocation(std::string location)
	{
		keyLocation.push_back(location);
	}

	bool SystemManager::PlayerManager::FindLocation(std::string location)
	{
		//bool find = false;
		for (std::string s : keyLocation) 
		{
			if (s._Equal(location)) 
			{
				return true;
				//find = true;
				//break;
			}
		}

		//return find;
		return false;
	}

	void SystemManager::PlayerManager::reset()
	{
		isBlue = false;
		health = 5;
		keys = 0;
		isRestrickMovement = false;
		isShakeEvent = false;
		keyLocation.clear();
		isTimerEvent = false;
		eventTimer = Timer(EVENT_COUNTDOWN_SEC * CLOCKS_PER_SEC);
		isTimerUp = false;
	}

	

}

