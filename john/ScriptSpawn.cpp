/******************************************************************************/
/*!
\file		SpawnScript.cpp
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
#include "Blast.h"
#include "SystemInput.h"
#include "ScriptSpawn.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include "SystemManager.h"
#include "Math_CollisionSAT.h"
#include <math.h>


FwEngine::ScriptSpawn::ScriptSpawn(ComponentScript& comp) : IScript(STRING_SCRIPT_SPAWN, comp),
colPhy(_components.getComponent<ComponentPhysics>()),
colAnimation(_components.getComponent<ComponentAnimation>()),
colRender(_components.getComponent<ComponentRenderer>()),
colTrans(_components.getComponent<ComponentTransform>()),
colCollision(_components.getComponent<ComponentCollision>()),
colAudio(_components.getComponent<ComponentAudio>()),
objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
sysMgr{ _components.getSystemManager() },
inp(_components.getSystemInput()),
sysScr(_components.getSystemScript()),
parentObjectId(_components.getParentGameObject()->GetIndex()),
parentObject(nullptr),
parentTransform(nullptr),
parentRenderer(nullptr),

_spawnBehaviour(SPAWNBEHAVIOUR_PASSIVE),
_limitBehaviour(LIMITBEHAVIOUR_NOLIMIT),
_spawnTimer(0),
_spawnLimit(0),
_spawnCount(0),

_isActive(true),

_sms(nullptr)
{
	parentObject = objectPool.GetGameObject(parentObjectId);
	parentTransform = objectPool.GetComponent<ComponentTransform>(parentObjectId);
	parentRenderer = objectPool.GetComponent<ComponentRenderer>(parentObjectId);

	// in order
	// floats
	AddFloatVar("Spawn Limit", &_spawnLimit);
	AddFloatVar("Spawn Time Interval", &_spawnTimeInterval);
	// string
	AddStringVar("Spawn Behaviour", &_spawnBehaviourString);
	AddStringVar("Limit Behaviour", &_limitBehaviourString);
	AddStringVar("Spawn prefap name", &_spawnName);

}

FwEngine::ScriptSpawn::~ScriptSpawn()
{
}

void FwEngine::ScriptSpawn::Init()
{
	_components.getParentGameObject()->GetIndex();

	std::string name = colTrans._tag;

	// set _spawnBehaviour

	if (name._Equal("player"))
	{
		// do sth maybe
	}

	// spawn behaviour
	if (_spawnBehaviourString._Equal("timer"))
	{
		_spawnBehaviour = SPAWNBEHAVIOUR_TIMER;
	}
	if (_spawnBehaviourString._Equal("switch"))
	{
		_spawnBehaviour = SPAWNBEHAVIOUR_SWITCH;
	}
	if (_spawnBehaviourString._Equal("passive"))
	{
		_spawnBehaviour = SPAWNBEHAVIOUR_PASSIVE;
	}

	// limit behaviour
	if (_limitBehaviourString._Equal("limitmax"))
	{
		_limitBehaviour = LIMITBEHAVIOUR_LIMITMAX;
	}
	if (_limitBehaviourString._Equal("destroyoldest"))
	{
		_limitBehaviour = LIMITBEHAVIOUR_DESTROYOLDEST;
	}
	if (_limitBehaviourString._Equal("nolimit"))
	{
		_limitBehaviour = LIMITBEHAVIOUR_NOLIMIT;
	}
	if (_limitBehaviourString._Equal("timed"))
	{
		_limitBehaviour = LIMITBEHAVIOUR_TIMED;
	}

	_spawnTimer = Timer((int)_spawnTimeInterval);
	_spawnTimer.reset();
	//_spawnLimit = _spawnLimit; // editor
	//_spawnName = _spawnName; // editor


	// Message
	_sms = sysScr._sms.TryRegister<ScriptMessage_Player>("player");

	//reinterpret_cast<ScriptMessage_Player*>(_sms)->movementParticle;
	//reinterpret_cast<ScriptMessage_Player*>(_sms)->velocity;

}

FwEngine::GameObject* FwEngine::ScriptSpawn::SpawnPrefap(std::string fapname)
{
	GameObject* newObj = sysMgr.CreatePrefabObject(fapname);

	if (newObj)
	{
		ComponentTransform* newObjectTransform = objectPool.GetComponent<ComponentTransform>(newObj->GetIndex());
		newObjectTransform->_currentPosition = FwMath::Vector3D(parentTransform->_currentPosition);
	}
	else
	{
		// failed to spawn i guess
	}

	return newObj;
}

void FwEngine::ScriptSpawn::Limit()
{
	switch (_limitBehaviour)
	{
	case LIMITBEHAVIOUR_NOLIMIT:
		// no limits
		// dun do anything
		break;
	case LIMITBEHAVIOUR_DESTROYOLDEST:
		// destroy oldest obj
		if (_spawnCount >= _spawnLimit)
		{
			PopObjectList();
		}
		break;
	case LIMITBEHAVIOUR_LIMITMAX:
		// if hit max limit dun spawn more
		if (_spawnCount >= _spawnLimit)
			return;
		break;
	case LIMITBEHAVIOUR_TIMED:
		Timer timer = _spawnObjectsTimers.front();
		if (timer.checkTrigger())
		{
			PopObjectList();
		}
		break;
	}
}

void FwEngine::ScriptSpawn::Spawn()
{
	switch (_spawnBehaviour)
	{
	case SPAWNBEHAVIOUR_PASSIVE:
		// dun spawn
		// dun do anything
		break;
	case SPAWNBEHAVIOUR_TIMER:
		if (_spawnTimer.checkTrigger())
		{
			PushObjectList();
			_spawnTimer.reset();
		}
		break;
	case SPAWNBEHAVIOUR_SWITCH:
		// do switch thingy

		break;

	default:
		break;
	}
}

void FwEngine::ScriptSpawn::PopObjectList()
{
	if (_spawnObjects.size() > 0)
	{
		GameObject* oldFap = _spawnObjects.front();
		if (oldFap)
		{
			oldFap->disable();
			_spawnObjects.pop_front();
			--_spawnCount;
			_spawnObjectsTimers.pop_front();
		}
	}
}

void FwEngine::ScriptSpawn::PushObjectList()
{
	GameObject* newFap = SpawnPrefap(_spawnName);
	++_spawnCount;
	_spawnObjects.push_back(newFap);

	AddSpawnTimer();
}

void FwEngine::ScriptSpawn::AddSpawnTimer()
{
	_spawnObjectsTimers.push_back(Timer((int)_spawnLimit));
	//if (_limitBehaviour == LIMITBEHAVIOUR_TIMED)
	//{
	//}
}

void FwEngine::ScriptSpawn::Update(float)
{
	// TODO

	//GameObject* newFap = SpawnPrefap("Background_Concrete001"); // spawns a new prefap using the fapname at the position of the current object

	if (_isActive)
	{
		Limit();
		Spawn();
	}


}

void FwEngine::ScriptSpawn::Free()
{
}

void FwEngine::ScriptSpawn::Destroy()
{
}

void FwEngine::ScriptSpawn::OnCollisionEnter(size_t)
{
}

void FwEngine::ScriptSpawn::OnCollisionExit(size_t)
{
}

void FwEngine::ScriptSpawn::OnCollisionStay(size_t)
{
	
}

void FwEngine::ScriptSpawn::PushVars()
{
	// in order
	// floats
	if (_originalVars.size() > 0) {
		_spawnLimit = std::stof(_originalVars[0]);
		_originalVars.erase(std::begin(_originalVars));
	}
	if (_originalVars.size() > 0)
	{
		_spawnTimeInterval = std::stof(_originalVars[0]);
		_originalVars.erase(std::begin(_originalVars));
	}
	// string
	if (_originalVars.size() > 0) {
		_spawnBehaviourString = _originalVars[0];
		_originalVars.erase(std::begin(_originalVars));
	}
	if (_originalVars.size() > 0)
	{
		_limitBehaviourString = _originalVars[0];
		_originalVars.erase(std::begin(_originalVars));
	}
	if (_originalVars.size() > 0) {
		_spawnName = _originalVars[0];
		_originalVars.erase(std::begin(_originalVars));
	}
}
