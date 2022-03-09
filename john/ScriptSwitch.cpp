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
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "ScriptSwitch.h"
#include "SystemManager.h"
#include "ScriptPlayer.h"
#include "SystemScript.h"

namespace FwEngine
{
	ScriptSwitch::ScriptSwitch(ComponentScript& comp) : IScript(STRING_SCRIPT_SWITCH, comp),
		_comTrans(_components.getComponent<ComponentTransform>()),
		_objectPool{ *(_components.getParentGameObject()->GetParentObjectPool()) },
		_inp(_components.getSystemInput()),
		_gfx(_components.getSystemGraphics()),
		_sysMgr(_components.getSystemManager()),
		_sysScr(_components.getSystemScript()),
		_currDoorName{},
		_currSwitchName{ _components.getParentGameObject()->GetName() },
		_playerKeys{ nullptr },
		_active{ 0 },
		_opened{ 0 },
		_keyRequirement{ 0 },
		_doorSpeed{ 1.0f },
		_doors{ },
		_doorOpenedPos{},
		_doorClosedPos{}
	{
		AddFloatVar("Door Speed", &_doorSpeed);
		AddIntVar("Open", &_opened);
		AddIntVar("Keys Requirement", &_keyRequirement);
	}

	ScriptSwitch::~ScriptSwitch()
	{
	}

	void ScriptSwitch::Init()
	{
		_currDoorName = std::string("Door").append(_currSwitchName.substr(_currSwitchName.find("Switch")+6));
		_doors = _objectPool.FindGameObjectsByName<ComponentTransform>(_currDoorName);
		GameObject* player = _objectPool.FindGameObjectByTag<GameObject>("player");

		IScript* scr = _sysScr.FindScript((int)player->GetIndex(), "player");
		ScriptPlayer* scrPlayer = reinterpret_cast<ScriptPlayer*>(scr);
		_playerKeys = scrPlayer->GetKey();

		size_t i = 0;
		for (ComponentTransform* trans : _doors)
		{
			_doorOpenedPos.push_back(trans->_currentPosition);
			_doorClosedPos.push_back(trans->_currentPosition);
			_doorOpenedPos[i].x = trans->_currentPosition.x;
			_doorOpenedPos[i].y = trans->_currentPosition.y + trans->_scale.y;
			++i;
		}
	}

	void ScriptSwitch::Update(float dt)
	{
		if (_active)
		{
			size_t i = 0;
			for (ComponentTransform* trans : _doors)
			{
				if (_opened && trans->_currentPosition.y < _doorOpenedPos[i].y)
				{
					trans->_currentPosition.y += _doorSpeed * dt;
				}
				else if (!_opened && trans->_currentPosition.y > _doorClosedPos[i].y)
				{
					trans->_currentPosition.y -= _doorSpeed * dt;
				}
				else
				{
					_active = 0;
				}
				++i;
			}
		}
	}

	void ScriptSwitch::Free()
	{
	}

	void ScriptSwitch::Destroy()
	{
	}

	void ScriptSwitch::OnCollisionEnter(size_t)
	{
	}

	void ScriptSwitch::OnCollisionExit(size_t)
	{

	}

	void ScriptSwitch::OnCollisionStay(size_t id)
	{
		ComponentTransform& otherTransform = _objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		if (otherTransform._tag == "player")
		{
			if ((_inp)(IKEY_W, SystemInput::PRESS) && _keyRequirement <= *_playerKeys)
			{
				_active = 1;
				_opened = !_opened;

				// event end
				_sysMgr._playerMgr.isTimerEvent = false;
			}
		}
	}

	void ScriptSwitch::PushVars()
	{
		if (_originalVars.size() > 0) {
			_doorSpeed = std::stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
		if (_originalVars.size() > 0)
		{
			_opened = std::stoi(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
		if (_originalVars.size() > 0)
		{
			_keyRequirement = std::stoi(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
	}
}

