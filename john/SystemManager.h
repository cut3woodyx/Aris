/******************************************************************************/
/*!
\file		SystemManager.h
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

#include "GameObjectPool.h"
#include "GameObjectFactory.h"
#include "SystemAudio.h"
#include "SystemPhysics.h"
#include "SystemGraphics.h"
#include "SystemInput.h"
#include "SystemScript.h"
#include "WindowSystem.h"
#include "Map.h"
#include <vector>
#include <string>
#include <map>

constexpr auto LOAD_SIZE = 100; // loading scn
constexpr auto FAB_SIZE = 100; // fabbbbbs

constexpr size_t SCENE_SZ = 100;

namespace FwEngine
{
	class SystemManager
	{
	protected:
		//============================= DO NOT MOVE AROUND =============================//
		float _currentDt;

		GameObjectPool _gameObjectPool;
		GameObjectFactory _gameObjectFactory;


		SystemPhysics _systemPhysics;
		SystemGraphics _systemGraphics;
		SystemAudio _systemAudio;
		SystemScript _systemScript;

		SystemInput _systemInput;

		std::vector<std::pair<std::string, LARGE_INTEGER>> _systemFuncTime;

	public:
		Map _map;
		FwMath::Vector3D _cursorPosition;

		//============================= DO NOT MOVE AROUND =============================//




	protected:
		bool _isPhysicsRunning;
		bool _isScriptRunning;

		// loading scn //

		GameObjectPool _loadingGameObjectPool;
		GameObjectFactory _loadingGameObjectFactory;

		// //


		// scenes // 
		// // // maybe in the future can implement classes to manage the seperate aspects of the game
		// // // like for example a scn mgr to manage the scn 


		std::string menu_objects;
		std::string menu_map;

		std::vector<std::string> filepath_objects;
		std::vector<std::string> filepath_map;

		std::vector<std::string> sceneNames;

		//std::vector<GameObjectPool> _gameObjectPools[SCENE_SZ];
		//std::vector<GameObjectFactory> _gameObjectFactorys[SCENE_SZ];

		//std::vector<bool> _gameObjectIsLoaded[SCENE_SZ];

		//std::map<std::string, GameObjectPool> _objectPoolMap; 
	public:
		std::list<std::string> _objectPoolNames;
	protected:

		std::string _currentObjPath;
		std::string _currentMapPath;

		// //

	public:
		std::string _nextScene;
		std::string _currentScene;

		std::string _prevScene;

		// // prefabs //

		GameObject* CreatePrefabObject(std::string name);

	protected:

		GameObjectPool _prefabPool;
		GameObjectFactory _prefabFac3;
		std::map<int, std::string> _prefabNames;
		
		bool _isFabsLoaded;

		void LoadFabs();

		void BuildFabs();

		// //


		// // // splash and /*loading scr*/

		std::vector<GameObject*> _objpVec;

		//GameObjectPool _anotherGameObjectPool; // 

		int _splashState = 0;

		//ComponentText* _debugComponent;

		GameObject* _splashObj;
		ComponentRenderer* _splashRender;

	public:

		const int ANOTHERMENU_INDEX = 0;
		const int MAINMENU_INDEX = 1;
		const int STARTLEVEL_INDEX = 2;

		//const int LEVELMAX_INDEX = 2;


		enum GAMESTATE
		{

			GAMESTATE_UPDATE,
			GAMESTATE_PAUSE,
			GAMESTATE_PAUSE_STEP,
			GAMESTATE_UPDATE_STEP,
			GAMESTATE_PAUSE_OPTIONS,

			GAMESTATE_LOAD,
			GAMESTATE_LOAD_LEVEL,
			GAMESTATE_LOAD_NEXTLEVEL,

			GAMESTATE_LOAD_NEXTSCENE,

			GAMESTATE_QUIT,

			GAMESTATE_LOAD_RETURNTOMENU,

			GAMESTATE_LOAD_MAINMENU,
			GAMESTATE_LOAD_ANOTHERMENU,
			GAMESTATE_LOAD_RELOAD,

			GAMESTATE_END
		};
		GAMESTATE _gamestate;
		GAMESTATE _gamestateNext;

		enum MENUSTATE
		{
			MENUSTATE_MAIN,
			MENUSTATE_OPTIONS,
			MENUSTATE_CREDITS,

			MENUSTATE_PLAYERDIED,
			MENUSTATE_PLAYERWON,

			MENUSTATE_END
		};
		MENUSTATE _menustate;
		MENUSTATE _menustateNext;

		int _level;
		int _levelMax;

		float _currentFPS;
		int _currentObj;

		bool _isDebug;

		bool _isTest;
		int _testCount;

		//Needed to pass handle
		WindowSystem* _windows;


		void PrintObjects();
		void LoadLevelConfig();

		//ComponentText* CreateDebugString();

		void SplashInit();
		void SplashUpdate(float dt);
		void SplashEnd();

		void FakeLoadingScrInit();
		void FakeLoadingScrEnd();

		void LoadingInit();

		void LoadingEnd();

		void LoadingUpdate(float dt);

		ComponentText* AddTextObject();


		class PlayerManager
		{
			const int EVENT_COUNTDOWN_SEC = 60;
			//const int EVENT_COUNTDOWN_SEC = 10;
		public:
			bool isBlue;
			int health;
			int keys;
		
			bool isRestrickMovement;
			bool isShakeEvent;
			std::list<std::string> keyLocation;

			bool isTimerEvent;
			Timer eventTimer;
			bool isTimerUp;

			PlayerManager() : 
				isBlue(false), 
				health(5), 
				keys(0), 
				isRestrickMovement(false), 
				isShakeEvent(false),
				isTimerEvent(false),
				eventTimer(EVENT_COUNTDOWN_SEC * CLOCKS_PER_SEC),
				isTimerUp(false)
			{};
			~PlayerManager() = default;
			void addKeyLocation(std::string location);
			bool FindLocation(std::string location);
			void reset();


		};
		PlayerManager _playerMgr;

	public:
		SystemManager();

		~SystemManager();

		virtual void Init();
		virtual void Load(std::string objectFilename, std::string mapFileName, bool disableScriptInit = false, bool useSavScn = false);
		virtual void Free();
		virtual int Update(float dt, int step = 1, bool disableDebugInputs = false);
		//void Destroy();

		friend class ComponentScript;
		friend class SystemScript;
	};

}

