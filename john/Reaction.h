/******************************************************************************/
/*!
\file		Reaction.h
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
#include "ComponentCollision.h"
#include "ForceManager.h"
namespace FwEngine
{


	void StackingResponse(
		ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1,
		ComponentPhysics* phys2, ComponentTransform* trans2, ComponentCollision* coll2
	);

	void StackingResponse_Elastic(
		ComponentPhysics* phy1, ComponentTransform* trans1, ComponentCollision* coll1,
		ComponentPhysics* phy2, ComponentTransform* trans2, ComponentCollision* coll2
	);

	void StackingResponse_NonElastic(
		ComponentPhysics* phy1, ComponentTransform* trans1, ComponentCollision* coll1, 
		ComponentPhysics* phy2, ComponentTransform* trans2, ComponentCollision* coll2
	);

	void StackingResponse_forcePushing(
		ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1,
		ComponentPhysics* phys2, ComponentTransform* trans2, ComponentCollision* coll2
	);

	void NoPhysicsUnpenetrableObject(
		ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1,
		ComponentTransform* trans2, ComponentCollision* coll2
	);

}