/******************************************************************************/
/*!
\file		ScriptForceField.cpp
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

constexpr auto STRING_SCRIPT_FORCE_FIELD = "force field";

namespace FwEngine
{
	class ScriptForceField : public IScript
	{
	protected:
		GameObjectPool& objectPool;
		DirectionalForce force;

	public:
		ScriptForceField(ComponentScript& comp);

		virtual ~ScriptForceField();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;


		virtual void OnCollisionStay(size_t id) override;
	};

}