/******************************************************************************/
/*!
\file		ScriptKillZone.cpp
\project	Aris
\author 	Primary: Wang YiDi
\par    	email: w.yidi\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "ScriptKillZone.h"
namespace FwEngine
{
	ScriptKillZone::ScriptKillZone(ComponentScript& comp):IScript(STRING_SCRIPT_KILL_ZONE, comp),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
		_playerPosition{ nullptr }
	{
		AddFloatVar("Pos X ##Kill", &_positionKill.x);
		AddFloatVar("Pos Y ##Kill", &_positionKill.y);
	}
	ScriptKillZone::~ScriptKillZone()
	{
	}
	void ScriptKillZone::Init()
	{
		ComponentTransform* player = &objectPool.GetContainerPtr<ComponentTransform>()->at(0);
		size_t playerId = 0;
		const size_t compSize = objectPool.GetContainerPtr<ComponentTransform>()->size();
		for (size_t i = 0; i < compSize; ++player, ++i)
		{
			if (player->_tag == "player")
			{
				playerId = i;
				_playerPosition = player;
				break;
			}
		}
	}
	void ScriptKillZone::Update(float)
	{
	}
	void ScriptKillZone::Free()
	{
	}
	void ScriptKillZone::Destroy()
	{
	}
	Vector3D& ScriptKillZone::returnKillPosition()
	{
		return _positionKill;
	}
	void ScriptKillZone::OnCollisionEnter(size_t id)
	{
		ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		if (otherTransform._tag == "player")
		{
			_playerPosition->_currentPosition = _positionKill;
		}
	}
	void ScriptKillZone::OnCollisionExit(size_t)
	{
	}
	void ScriptKillZone::OnCollisionStay(size_t)
	{
	}

	void ScriptKillZone::PushVars()
	{
		if (_originalVars.size() > 0) {
			_positionKill.x = std::stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
		if (_originalVars.size() > 0)
		{
			_positionKill.y = std::stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
	}
}