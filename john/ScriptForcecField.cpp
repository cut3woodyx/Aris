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
#include "ScriptForceField.h"

namespace FwEngine
{
	ScriptForceField::ScriptForceField(ComponentScript& comp):
		IScript(STRING_SCRIPT_FORCE_FIELD, comp),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool()))
	{
	}
	ScriptForceField::~ScriptForceField()
	{
	}
	void ScriptForceField::Init()
	{
		force = DirectionalForce((0.0f, 1.0f, 0.0f), 3000.0f, 0.1f);
	}
	void ScriptForceField::Update(float)
	{
	}
	void ScriptForceField::Free()
	{
	}
	void ScriptForceField::Destroy()
	{
	}


	void ScriptForceField::OnCollisionStay(size_t id)
	{
		ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);

		if (otherTransform._tag=="player")
		{ 
			ComponentPhysics& playerPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
			playerPhysics.AddForce(force);
		}
	}

}