/******************************************************************************/
/*!
\file		Math_CollisionAABB.h
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

#include "FWMath.h"


namespace FwMath
{

	/**************************************************************************/
	/*!

	*/
	/**************************************************************************/


	struct AABB
	{
		Vector3D	_min;
		Vector3D	_max;
	};

	struct Circle
	{
		Vector3D	_position;
		float _radius;
	};

	bool CollisionIntersection_RectRect(const AABB& aabb1, const Vector3D& vel1,
		const AABB& aabb2, const Vector3D& vel2, const float dt, Vector3D& normal);

	bool CollisionIntersection_RectCircle(const AABB& rectangle, const Vector3D& positionRectangle,
		const Circle& circle, const Vector3D& positionCircle, float& penetration, Vector3D& normal);
}
