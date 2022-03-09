/******************************************************************************/
/*!
\file		LESysMgr.h
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

#ifndef _LE_SYSTEM_MANAGER
#define _LE_SYSTEM_MANAGER

#include "GameObjectPool.h"
#include "GameObjectFactory.h"
#include "SystemGraphics.h"
#include "SystemInput.h"
#include "SystemPhysics.h"
#include "SystemScript.h"
#include "SystemManager.h"
#include "WindowSystem.h"
#include "Map.h"
#include "imguiMgr.h"

namespace FwEngine
{
	class LESysMgr : public SystemManager
	{
		friend class ImguiMgr;
	private:
		ImguiMgr _imguiMgr;

		void CameraControl();

	public:

		LESysMgr();

		~LESysMgr();

		void Init() override;
		void Load(std::string objFilename = std::string{ "" }, std::string mapFilename = std::string{ "" }, bool disableScriptInit = false, bool useSavScn = false) override;
		int Update(float dt, int step = 1, bool disableDebugInputs = false) override;
		void Free() override;
	};

	//class LESysMgr
	//{
	//	friend class ImguiMgr;
	//	//============================= DO NOT MOVE AROUND =============================//
	//	float _currentDt;

	//	GameObjectPool _gameObjectPool;
	//	GameObjectFactory _gameObjectFactory;

	//	SystemPhysics _systemPhysics;
	//	SystemGraphics _systemGraphics;

	//	SystemScript _systemScript;

	//	SystemInput _systemInput;

	//	Map _map;
	//public:
	//	FwMath::Vector3D _cursorPosition;
	//	//============================= DO NOT MOVE AROUND =============================//
	//private:
	//	ImguiMgr _imguiMgr;

	//	void CameraControl();

	//public:
	//	//Needed to pass handle
	//	WindowSystem* _windows;

	//	LESysMgr();

	//	~LESysMgr();

	//	void Init();
	//	int Update(float dt, int step = 1);
	//	void Load(std::string objFilename = std::string{ "" }, std::string mapFilename =std::string{ "" });
	//	void Free();
	//};

}

#endif;
