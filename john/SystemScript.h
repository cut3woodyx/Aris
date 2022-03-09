/******************************************************************************/
/*!
\file		SystemScript.h
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

#include "ISystem.h"
#include  "ComponentScript.h"
#include <vector>
#include "IScript.h"

namespace FwEngine
{

	class SystemScript : public ISystem
	{
		//typedef IComponent* (*FunctionGetComponent)(size_t index, Component com);
	public:
		//FunctionGetComponent functionGetComponent;

		SystemManager* _systemManager;

		std::vector<GameObjectPool*> _otherPools;

		ScriptMessagingSystem _sms;

		SystemScript();
		SystemScript(GameObjectPool* gameObjectPool);
		~SystemScript();

		IScript* FindScript(int objId, std::string scriptName);

		void InitAllScripts();

		void OnEnterScene();

		void UpdateMenu(float dt);

		void Init() override;
		void Update(float dt) override;
		void Unload();
		void Free() override;
		void Destroy() override;
	};

	extern SystemGraphics* GRAPHICS;
}

