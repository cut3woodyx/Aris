/******************************************************************************/
/*!
\file		Math_CollisionAABB.cpp
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
#include "Math_CollisionAABB.h"
#include <algorithm>

using std::min;
using std::max;

namespace FwMath
{


	/**************************************************************************/
	/*!

	*/
	/**************************************************************************/
	bool CollisionIntersection_RectRect(
		const AABB& aabb1, const Vector3D& vel1,
		const AABB& aabb2, const Vector3D& vel2, 
		const float dt, Vector3D& normal)
	{

		if (!(aabb1._max.x < aabb2._min.x || aabb1._min.x > aabb2._max.x ||
			aabb1._max.y < aabb2._min.y || aabb1._min.y > aabb2._max.y))
		{
			return true;
		}

		//		If the check returns no overlap you continue with the following steps.

		//Step 2: Initialize and calculate the new velocity of Vb
		//		tFirst = 0
		//		tLast = dt
		Vector3D relativeVelocity{0};
		relativeVelocity.x = vel2.x - vel1.x;
		relativeVelocity.y = vel2.y - vel1.y;

		float tFirst = 0;
		float tLast = dt;

		//Step 3: Working with one dimension (x-axis).
		//		if(Vb < 0)
		if (relativeVelocity.x < 0)
		{
			//			case 1: No intersection
			if (aabb1._min.x > aabb2._max.x)
			{
				return false;
			}

			//			case 4: Moving towards each other
			if (aabb1._max.x < aabb2._min.x)
			{
				tFirst = max((aabb1._max.x - aabb2._min.x) / relativeVelocity.x, tFirst);
			}
			if (aabb1._min.x < aabb2._max.x)
			{
				tLast = min((aabb1._min.x - aabb2._max.x) / relativeVelocity.x, tLast);
			}
			normal = Vector3D{ -1,0,0 };
		}
		//		if(Vb > 0)
		else if (relativeVelocity.x > 0)
		{
			//			case 3: No intersection
			if (aabb1._max.x < aabb2._min.x)
			{
				return false;
			}

			//			case 2: Moving towards each other
			if (aabb1._min.x > aabb2._max.x)
			{
				tFirst = max((aabb1._min.x - aabb2._max.x) / relativeVelocity.x, tFirst);
			}
			if (aabb1._max.x > aabb2._min.x)
			{
				tLast = min((aabb1._max.x - aabb2._min.x) / relativeVelocity.x, tLast);
			}
			normal = Vector3D{ 1,0,0 };
		}
		else
		{
			if (aabb1._min.x > aabb2._max.x)
			{
				return false;
			}
			else if (aabb1._max.x < aabb2._min.x)
			{
				return false;
			}
		}

		//		case 5: No intersection
		if (tFirst > tLast)
		{
			return false;
		}

		//Step 4: Repeat step 3 on the y-axis 
		//Step 3: Working with one dimension (y-axis).
		//		if(Vb < 0)
		if (relativeVelocity.y < 0)
		{
			//			case 1: No intersection
			if (aabb1._min.y > aabb2._max.y)
			{
				return false;
			}

			//			case 4: Moving towards each other
			if (aabb1._max.y < aabb2._min.y)
			{
				tFirst = max((aabb1._max.y - aabb2._min.y) / relativeVelocity.y, tFirst);
			}
			if (aabb1._min.y < aabb2._max.y)
			{
				tLast = min((aabb1._min.y - aabb2._max.y) / relativeVelocity.y, tLast);
			}
			normal = Vector3D{ 0,1,0 };
		}
		//		if(Vb > 0)
		else if (relativeVelocity.y > 0)
		{
			//			case 3: No intersection
			if (aabb1._max.y < aabb2._min.y)
			{
				return false;
			}

			//			case 2: Moving towards each other
			if (aabb1._min.y > aabb2._max.y)
			{
				tFirst = max((aabb1._min.y - aabb2._max.y) / relativeVelocity.y, tFirst);
			}
			if (aabb1._max.y > aabb2._min.y)
			{
				tLast = min((aabb1._max.y - aabb2._min.y) / relativeVelocity.y, tLast);
			}
			normal = Vector3D{ 0,-1,0 };
		}
		else
		{
			if (aabb1._min.y > aabb2._max.y)
			{
				return false;
			}
			else if (aabb1._max.y < aabb2._min.y)
			{
				return false;
			}
		}

		//		case 5: No intersection
		if (tFirst > tLast)
		{
			return false;
		}

		//Step 5: Otherwise the rectangles intersect
		return true;
	}

	bool CollisionIntersection_RectCircle(const AABB& rectangle, const Vector3D& positionRectangle,
		const Circle& circle, const Vector3D& positionCircle, float& penetration, Vector3D& normal)
	{
		float radius = circle._radius;
		Vector3D p = positionCircle;
		Vector3D minV = rectangle._min;
		Vector3D maxV = rectangle._max;
		Vector3D q;


		for (int i = 0; i < 2; ++i)
		{
			float v = p[i];
			if (v < minV[i]) v = minV[i];
			if (v > maxV[i]) v = maxV[i];
			q[i] = v;
		}
		Vector3D delta = p - q;
		float disSq = VectorSquareLength(delta);
		if (disSq < radius * radius)
		{
			if (disSq == 0.0f)
			{
				//collision on edge
				Vector3D bdelta = positionCircle - positionRectangle;
				Vector3D axis = { (rectangle._max.x - rectangle._min.x) / 2,(rectangle._max.y - rectangle._min.y) / 2 ,0 };

				float xd = radius + axis.x - fabs(bdelta.x);
				float yd = radius + axis.y - fabs(bdelta.y);

				if (xd < yd)
				{
					normal = bdelta.x < 0 ? Vector3D(-1, 0, 0) : Vector3D(1, 0, 0);
					penetration = xd;
				}
				else
				{
					normal = bdelta.y < 0 ? Vector3D(0, -1, 0) : Vector3D(0, 1, 0);
					penetration = yd;
				}
				return true;
			}
			else
			{
				//circle is inside square
				float dis = (float)delta.length();
				normal = delta;
				penetration = -(dis - circle._radius);
				return true;
			}
		}

		return false;
	}
}
