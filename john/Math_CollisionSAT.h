/******************************************************************************/
/*!
\file		Math_CollisionSAT.h
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

namespace FwMath
{
	//Collision flags
	const unsigned int	COLLISION_LEFT = 0x00000001;	//0001
	const unsigned int	COLLISION_RIGHT = 0x00000002;	//0010
	const unsigned int	COLLISION_TOP = 0x00000004;	//0100
	const unsigned int	COLLISION_BOTTOM = 0x00000008;	//1000

	bool CollisionIntersection_SAT_projection(
		const FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1,
		const FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2);

	bool CollisionIntersection_SAT_DotProduct(
		const FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1,
		const FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2);

	bool CollisionIntersection_SAT_Intersection(
		const FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1,
		const FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2);

	void checkCollisionSides(
		FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1,
		FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2,
		int & flag);
}