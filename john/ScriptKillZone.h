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
#pragma once
#include "IScript.h"

constexpr auto STRING_SCRIPT_KILL_ZONE = "kill_zone";

namespace FwEngine
{
	class ScriptKillZone : public IScript
	{
	protected:
		GameObjectPool& objectPool;
		ComponentTransform* _playerPosition;
		Vector3D _positionKill;

	public:
		ScriptKillZone(ComponentScript& comp);
		
		virtual ~ScriptKillZone();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		Vector3D& returnKillPosition();

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void PushVars() override;
	};

}