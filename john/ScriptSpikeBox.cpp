/******************************************************************************/
/*!
\file		ScriptSpikeBox.cpp
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
#include "ScriptSpikeBox.h"
#include "SystemScript.h"

namespace FwEngine
{
	ScriptSpikeBox::ScriptSpikeBox(ComponentScript& comp) :
		IScript(STRING_SCRIPT_SPIKE_BOX, comp),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
		colCollision(_components.getComponent<ComponentCollision>()),
		pScr{nullptr},
		side(0)
	{
	}

	ScriptSpikeBox::~ScriptSpikeBox()
	{
	}
	void ScriptSpikeBox::Init()
	{
		size_t pindex = objectPool.FindGameObjectByTag<GameObject>("player")->GetIndex();
		SystemScript& sysScr = _components.getSystemScript();
		pScr = reinterpret_cast<ScriptPlayer*> (sysScr.FindScript((int)pindex, "player"));
		
	}
	void ScriptSpikeBox::Update(float)
	{
	}
	void ScriptSpikeBox::Free()
	{
	}
	void ScriptSpikeBox::Destroy()
	{
	}
	void ScriptSpikeBox::OnCollisionEnter(size_t)
	{

	}
	void ScriptSpikeBox::OnCollisionExit(size_t)
	{

	}
	void ScriptSpikeBox::OnCollisionStay(size_t id)
	{
		ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);

		if (otherTransform._tag == "player")
		{
			
			char check = colCollision.Collision_Check_List.find("player")->second;
			if (check == side ||
				check == 'T' ||
				check == 'B')
			{
				pScr->_health -= 1;
			}
		}

	}
}