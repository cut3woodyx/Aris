/******************************************************************************/
/*!
\file		SpawnScript.h
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
#include "IScript.h"
#include <ctime>



constexpr auto STRING_SCRIPT_SPAWN = "spawn";

namespace FwEngine
{
	class SystemScript;

	class ScriptSpawn : public IScript
	{
	protected:
		ComponentPhysics& colPhy;
		ComponentAnimation& colAnimation;
		ComponentRenderer& colRender;
		ComponentTransform& colTrans;
		ComponentCollision& colCollision;
		ComponentAudio& colAudio;
		GameObjectPool& objectPool;
		SystemManager& sysMgr;
		SystemInput& inp;
		SystemScript& sysScr;
		size_t parentObjectId;
		GameObject* parentObject;
		ComponentTransform* parentTransform;
		ComponentRenderer* parentRenderer;


		enum SpawnBehaviour
		{
			SPAWNBEHAVIOUR_PASSIVE,
			SPAWNBEHAVIOUR_TIMER,
			SPAWNBEHAVIOUR_SWITCH
		};
		SpawnBehaviour _spawnBehaviour;
		std::string _spawnBehaviourString; // editor var

		enum LimitBehaviour
		{
			LIMITBEHAVIOUR_NOLIMIT,
			LIMITBEHAVIOUR_DESTROYOLDEST,
			LIMITBEHAVIOUR_LIMITMAX,

			LIMITBEHAVIOUR_TIMED
		};
		LimitBehaviour _limitBehaviour;
		std::string _limitBehaviourString; // editor var

		Timer _spawnTimer;
		
		std::string _spawnName; // editor var
		float _spawnLimit; // editor var
		float _spawnTimeInterval; // editor var

		int _spawnCount;
		bool _isActive;
		std::list<GameObject*> _spawnObjects;
		std::list<Timer> _spawnObjectsTimers;

		GameObject* SpawnPrefap(std::string fapname);

		void PopObjectList();
		void PushObjectList();
		void AddSpawnTimer();

		void Limit();
		void Spawn();


		// sms
		ScriptMessageObject* _sms;


	public:

		ScriptSpawn(ComponentScript& comp);

		virtual ~ScriptSpawn();

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
