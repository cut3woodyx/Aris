/******************************************************************************/
/*!
\file		ScriptNonMetal.cpp
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
#include "ScriptNonMetal.h"
#include "Reaction.h"
namespace FwEngine
{
	ScriptNonMetal::ScriptNonMetal(ComponentScript& comp) :IScript(STRING_SCRIPT_NON_METAL, comp),
		colAnim{ _components.getComponent<ComponentAnimation>() },
		colPhy{ _components.getComponent<ComponentPhysics>() }
		/*
		colCollision(_components.getComponent<ComponentCollision>()),
		colTransform(_components.getComponent<ComponentTransform>()),
		colPhysics(_components.getComponent<ComponentPhysics>()),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool()))
		*/
	{
	}

	ScriptNonMetal::~ScriptNonMetal()
	{
	}
	void ScriptNonMetal::Init()
	{
	}
	void ScriptNonMetal::Update(float)
	{
		// Going left
		if (colPhy._velocity.x < -1.0f)
		{
			colAnim.PlayAnimation("");
		}
		// Going right
		else if (colPhy._velocity.x > 1.0f)
		{
			colAnim.PlayAnimation("");
		}
		// Not moving
		else
		{
			colAnim.PlayAnimation("");
		}
	}
	void ScriptNonMetal::Free()
	{
	}
	void ScriptNonMetal::Destroy()
	{
	}
	void ScriptNonMetal::OnCollisionEnter(size_t)
	{

	}
	void ScriptNonMetal::OnCollisionExit(size_t)
	{

	}
	void ScriptNonMetal::OnCollisionStay(size_t)
	{
		/*
		ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);

		if (otherTransform._tag == "non metal")
		{
			ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
			ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
			StackingResponse_NonElastic(&colPhysics, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);
		}
		if (otherTransform._tag == "metal")
		{
			ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
			ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
			StackingResponse_NonElastic(&colPhysics, &colTransform, &colCollision, &otherPhysics, &otherTransform, &otherCollision);
		}
		*/
	}
}