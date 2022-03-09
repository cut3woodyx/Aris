/******************************************************************************/
/*!
\file		ScriptSwitch.cpp
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	January 28, 2020
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include "IScript.h"

constexpr auto STRING_SCRIPT_SWITCH = "switch";

namespace FwEngine
{
	class SystemScript;

	class ScriptSwitch : public IScript
	{
	protected:
		ComponentTransform& _comTrans;
		GameObjectPool& _objectPool;
		SystemInput& _inp;
		SystemGraphics& _gfx;
		SystemManager& _sysMgr;
		SystemScript& _sysScr;
		std::vector<ComponentTransform*> _doors;

		std::string _currDoorName;
		std::string _currSwitchName;
		int* _playerKeys;

	public:

		int _active;
		int _opened;
		int _keyRequirement;
		float _doorSpeed;
		std::vector <FwMath::Vector3D> _doorOpenedPos;
		std::vector <FwMath::Vector3D> _doorClosedPos;
		

		ScriptSwitch(ComponentScript& comp);

		virtual ~ScriptSwitch();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
		virtual void PushVars() override;
	};

}
