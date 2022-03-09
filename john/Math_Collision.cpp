/******************************************************************************/
/*!
\file		Math_Collision.cpp
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
#include "Framework.h"
#include "Math_Collision.h"

#include <cmath>
#include <algorithm>

using std::min;


namespace FwMath
{


	/******************************************************************************/
	/*!
		Check for collision between a lineSegment and a circle
	 */
	 /******************************************************************************/
	int CollisionIntersection_CircleLineSegment(const Circle& circle,
		const Vector3D& ptEnd,
		const LineSegment& lineSeg,
		Vector3D& interPt,
		Vector3D& normalAtCollision,
		float& interTime,
		bool& checkLineEdges)
	{

		/* Velocity V */
		Vector3D v = ptEnd - circle.m_center;

		/* M is the outward normal to Velocity V */
		Vector3D m;
		m.x = v.y;
		m.y = -v.x;


		/* N.Bs - N.p0 = N.Bs_p0 */
		float nBsp0 = VectorDot(lineSeg.m_normal, circle.m_center) -
			VectorDot(lineSeg.m_normal, lineSeg.m_pt0);

		/* Here we consider we have an imaginary line LNS1, distant by 乏 (opposite N direction) */
		if (nBsp0 <= -circle.m_radius)
		{
			/* Check if the velocity vector V is within the end points of LNS1 */
			/* M is the outward normal to Velocity V . Compute P0・and P1・*/

			/* To simulate LNS1 line edge points */
			Vector3D p0_ = lineSeg.m_pt0 - circle.m_radius * lineSeg.m_normal;
			Vector3D p1_ = lineSeg.m_pt1 - circle.m_radius * lineSeg.m_normal;

			/* M.BsP0' * M.BsP1' */
			float mBsp0_mBsp1_ = VectorDot(m, p0_ - circle.m_center) *
				VectorDot(m, p1_ - circle.m_center);
			if (mBsp0_mBsp1_ < 0)
			{

				/* Ti = (N.P0 - N.Bs + R) / (N.V) //We are sure N.V != 0 */

				/* n.v */
				float nv = VectorDot(lineSeg.m_normal, v);
				if (nv == 0)
				{
					//throw "GG";
					return 0;
				}

				/* Ti = (N.P0 - N.Bs + R) / (N.V) */
				interTime = (VectorDot(lineSeg.m_normal, lineSeg.m_pt0 - circle.m_center) -
					circle.m_radius) / nv;

				if (0 <= interTime && interTime <= 1)
				{
					/* Bi = Bs + V *(Ti) */
					interPt = circle.m_center + v * interTime;

					/* Normal of reflection is -N */
					normalAtCollision = -lineSeg.m_normal;

					/* B'e = ApplyReflection(-N, BiBe) //Normal of reflection is -N */
					return 1;
				}
			}
			else
			{
				/* CheckMovingCircleToLineEdge(false) */
				checkLineEdges = false;
				return CheckMovingCircleToLineEdge(checkLineEdges,
					circle,
					ptEnd,
					lineSeg,
					interPt,
					normalAtCollision,
					interTime);
				//return 0;
			}
		}
		/* Here we consider we have an imaginary line LNS2 distant by +R (Same N direction) */
		else if (nBsp0 >= circle.m_radius)
		{
			/* Check if the velocity vector V is within the end points of LNS2 */
			/* M is the outward normal to Velocity V . Compute P0・and P1・*/

			/* To simulate LNS1 line edge points */
			Vector3D p0_ = lineSeg.m_pt0 + circle.m_radius * lineSeg.m_normal;
			Vector3D p1_ = lineSeg.m_pt1 + circle.m_radius * lineSeg.m_normal;

			/* M.BsP0' * M.BsP1' */
			float mBsp0_mBsp1_ = VectorDot(m, p0_ - circle.m_center) *
				VectorDot(m, p1_ - circle.m_center);
			if (mBsp0_mBsp1_ < 0)
			{

				/* Ti = (N.P0 - N.Bs + R) / (N.V) //We are sure N.V != 0 */

				/* n.v */
				float nv = VectorDot(lineSeg.m_normal, v);
				if (nv == 0)
				{
					//throw "GG";
					return 0;
				}

				/* Ti = (N.P0 - N.Bs + R) / (N.V) */
				interTime = (VectorDot(lineSeg.m_normal, lineSeg.m_pt0 - circle.m_center) +
					circle.m_radius) / nv;

				if (0 <= interTime && interTime <= 1)
				{
					/* Bi = Bs + V *(Ti) */
					interPt = circle.m_center + v * interTime;

					/* Normal of reflection is N */
					normalAtCollision = lineSeg.m_normal;

					/* B'e = ApplyReflection(N, BiBe) //Normal of reflection is N */
					return 1;
				}
			}
			else
			{
				/* CheckMovingCircleToLineEdge(false) */
				checkLineEdges = false;
				return CheckMovingCircleToLineEdge(checkLineEdges,
					circle,
					ptEnd,
					lineSeg,
					interPt,
					normalAtCollision,
					interTime);
				//return 0;
			}
		}
		else
		{
			/* The circle starting position Bs, is between both lines LNS1 and LNS2 */
			/* CheckMovingCircleToLineEdge(true) */
			checkLineEdges = true;
			return CheckMovingCircleToLineEdge(checkLineEdges,
				circle,
				ptEnd,
				lineSeg,
				interPt,
				normalAtCollision,
				interTime);
			//return 0;
		}

		return 0; // no intersection
	}

	/******************************************************************************/
	/*!
		Check for collision of a circle on the edge of a lineSegment
	*/
	/******************************************************************************/
	int CheckMovingCircleToLineEdge(bool withinBothLines,
		const Circle& circle,
		const Vector3D& ptEnd,
		const LineSegment& lineSeg,
		Vector3D& interPt,
		Vector3D& normalAtCollision,
		float& interTime)
	{

		/* Velocity V */
		Vector3D v = ptEnd - circle.m_center;

		/* M is the outward normal to Velocity V */
		Vector3D m;
		m.x = v.y;
		m.y = -v.x;

		/* um is unit vector of m */
		Vector3D um;
		VectorNorm(um, m);

		/* When _ true, is to say that Bs is starting from between both imaginary lines */
		if (withinBothLines)
		{
			/* Check which edge may collide first? */

			/* Bs p0 */
			Vector3D bsp0 = lineSeg.m_pt0 - circle.m_center;

			/* Bs p0 . p0 p1 */
			float bsp0_p0p1 = VectorDot(bsp0, lineSeg.m_pt1 - lineSeg.m_pt0);

			if (bsp0_p0p1 > 0) /*P0 side*/
			{
				/* BsP0.V */
				Vector3D uv;
				VectorNorm(v, uv);
				float m_f = VectorDot(bsp0, uv);

				if (m_f < 0) /* No collision */
				{
					return 0;
				}

				/* Reaching here means the circle movement is facing P0 */
				/* M is normalized outward normal of V */

				/* Same as P0.M - Bs.M (Shortest distance from P0 to V ) */
				float dist0 = VectorDot(bsp0, um);

				if (abs(dist0) > circle.m_radius)
				{
					return 0; /* no collision */
				}

				/* Reaching here means the circle movement is going towards P0 */
				/* The next line assumes the circle at collision time with P0 */
				float h = sqrt(circle.m_radius * circle.m_radius - dist0 * dist0);

				float vLen = (float)v.length();
				if (vLen == 0)
				{
					//throw "GG";
					return 0;
				}

				interTime = (m_f - h) / vLen;
				if (interTime <= 1)
				{
					interPt = circle.m_center + v * interTime;
					/* Normal of reflection is P0Bi normalized */
					/* B'e = ApplyReflection(P0Bi, BiBe) */
					
					VectorNorm(interPt - lineSeg.m_pt0, normalAtCollision);

					return 1;
				}
			}
			else /* (BsP1.P0P1 < 0) //P1 side */
			{
				/* Bs p1 */
				Vector3D bsp1 = lineSeg.m_pt1 - circle.m_center;

				/* BsP0.V */
				Vector3D uv;
				VectorNorm(v, uv);
				float m_f = VectorDot(bsp1, uv);

				if (m_f < 0) /* No collision */
				{
					return 0;
				}

				/* Reaching here means the circle movement is facing P1 */
				/* M is normalized outward normal of V */

				/* Same as P1.M - Bs.M */
				float dist1 = VectorDot(bsp1, um);

				if (abs(dist1) > circle.m_radius)
				{
					return 0; /* no collision */
				}

				/* Reaching here means the circle movement is going towards P1 */
				/* The next line assumes the circle at collision time with P1 */
				float h = sqrt(circle.m_radius * circle.m_radius - dist1 * dist1);

				float vLen = (float)v.length();
				if (vLen == 0)
				{
					//throw "GG";
					return 0;
				}

				interTime = (m_f - h) / vLen;
				if (interTime <= 1)
				{
					interPt = circle.m_center + v * interTime;
					/* Normal of reflection is P1Bi normalized */
					/* B'e = ApplyReflection(P1Bi, BiBe) */
					VectorNorm(interPt - lineSeg.m_pt1, normalAtCollision);

					return 1;
				}
			}

		}
		else /* else of: if(withinBothLines) */
		{
			/* Check which line edge, P0 or P1, is closer to the velocity vector V ? */
			bool p0Side = false;
			/* Bs P0 */
			Vector3D bsp0 = lineSeg.m_pt0 - circle.m_center;
			/* Bs P1 */
			Vector3D bsp1 = lineSeg.m_pt1 - circle.m_center;

			float dist0 = VectorDot(bsp0, um); /* Same as P0.M - Bs.M (M is normalized outward normal of V ) */
			float dist1 = VectorDot(bsp1, um); /* Same as P1.M - Bs.M */

			float dist0_absoulteValue = abs(dist0);
			float dist1_absoulteValue = abs(dist1);

			Vector3D uv;
			VectorNorm(uv, v);

			if (
				(dist0_absoulteValue > circle.m_radius) &&
				(dist1_absoulteValue > circle.m_radius)
				)
				return 0; // no collision
			else if (
				(dist0_absoulteValue <= circle.m_radius) &&
				(dist1_absoulteValue <= circle.m_radius)
				)
			{
				float m0 = VectorDot(bsp0, uv);
				float m1 = VectorDot(bsp1, uv);
				float m0_absoulteValue = abs(m0);
				float m1_absoulteValue = abs(m1);
				if (m0_absoulteValue < m1_absoulteValue)
					p0Side = true;
				else
					p0Side = false;
			}
			else if (dist0_absoulteValue <= circle.m_radius)
				p0Side = true;
			else /* if(dist1_absoluteValue <= R) */
				p0Side = false;

			if (p0Side) /* circle is closer to P0 */
			{
				float bsp0_v = VectorDot(bsp0, uv);
				if (bsp0_v < 0)
				{
					return 0; // no collision, moving away
				}
				else
				{
					/* Reaching here means the circle movement is going towards P0 */
					/* The next line assumes the circle at collision time with P0 */
					float h = sqrt(circle.m_radius * circle.m_radius - dist0 * dist0);

					float vLen = (float)v.length();
					if (vLen == 0)
					{
						//throw "GG";
						return 0;
					}

					interTime = (bsp0_v - h) / vLen;
					if (interTime <= 1)
					{
						interPt = circle.m_center + v * interTime;
						/* Normal of reflection is P0Bi normalized */
						/* B'e = ApplyReflection(P0Bi, BiBe) */
						VectorNorm(interPt - lineSeg.m_pt0, normalAtCollision);

						return 1;
					}
				}
			}
			else /* circle is closer to P1 */
			{
				float bsp1_v = VectorDot(bsp1, uv);
				if (bsp1_v < 0)
				{
					return 0; // no collision, moving away
				}
				else
				{
					/* Reaching here means the circle movement is going towards P1 */
					/* The next line assumes the circle at collision time with P1 */
					float h = sqrt(circle.m_radius * circle.m_radius - dist1 * dist1);

					float vLen = (float)v.length();
					if (vLen == 0)
					{
						//throw "GG";
						return 0;
					}

					interTime = (bsp1_v - h) / vLen;
					if (interTime <= 1)
					{
						interPt = circle.m_center + v * interTime;
						/* Normal of reflection is P1Bi normalized */
						/* B'e = ApplyReflection(P1Bi, BiBe) */
						VectorNorm(interPt - lineSeg.m_pt1, normalAtCollision);
						

						return 1;
					}
				}
			}

		}

		return 0;//no collision
	}

	/******************************************************************************/
	/*!
		Check for collision between a circle and a circle
	 */
	 /******************************************************************************/
	int CollisionIntersection_CircleCircle(const Circle& circleA,
		const Vector3D& velA,
		const Circle& circleB,
		const Vector3D& velB,
		Vector3D& interPtA,
		Vector3D& interPtB,
		float& interTime)
	{

		/* taking reference from circleB (circleB is stationary) */
		Ray ray;
		ray.m_dir = velA - velB;
		ray.m_pt0 = circleA.m_center;

		Circle circleStationary = circleB;
		circleStationary.m_radius += circleA.m_radius;

		int collision = CollisionIntersection_RayCircle(
			ray,
			circleStationary,
			interTime
		);

		if (collision)
		{
			interPtA = circleA.m_center + velA * interTime;
			interPtB = circleB.m_center + velB * interTime;
		}

		return collision;
	}

	/******************************************************************************/
	/*!
		Check for collision between a ray and a circle
	 */
	 /******************************************************************************/
	int CollisionIntersection_RayCircle(const Ray& ray,
		const Circle& circle,
		float& interTime)
	{

		float radiusSq = circle.m_radius * circle.m_radius;

		/* BsC = C - Bs*/
		Vector3D Bs_C = circle.m_center - ray.m_pt0;

		Vector3D uv;
		VectorNorm(ray.m_dir, uv);

		/* test for non-collision */
		/* compute projection of BsC onto uv */
		float proj_Bsc_uv = VectorDot(Bs_C, uv);

		if (proj_Bsc_uv < 0 && VectorSquareDistance(ray.m_pt0, circle.m_center) > radiusSq) /* && Bs outside circle */
		{
			/* circle behind ray origin */
			return 0;
		}

		float n2 = VectorSquareLength(Bs_C) - (proj_Bsc_uv * proj_Bsc_uv);
		if (n2 > radiusSq)
		{
			/* ray will miss the circle */
			return 0;
		}

		/*
			(C - B(t)) . (C - B(t)) = r^2
			(C - Bs - tv) . (C - Bs - tv) = r^2
			(t^2) (v.v) - 2(t)(C - Bs).v + (C - Bs).(C - Bs) - r^2 = 0

			at^2 + bt + c = 0

			a = v.v
			b = -2(C - Bs).v
			c = (C - Bs).(C - Bs) - r^2
		*/

		float a = VectorDot(ray.m_dir, ray.m_dir);
		float b = -2.0f * VectorDot(Bs_C, ray.m_dir);
		float c = VectorDot(Bs_C, Bs_C) - radiusSq;

		/*
			solve for t
			t = ( -b +- sqrt(b^2 - 4ac) ) / 2a
		*/

		float b2m4ac = (b * b) - (4.0f * a * c);

		if (b2m4ac < 0)
		{
			return 0; /* ray misses circle */
		}

		float t0 = 0;

		if (b2m4ac == 0)
		{
			/* ray grazes circle */
			t0 = -b / (2.0f * a);
		}
		else
		{
			float t1 = (-b + sqrt(b2m4ac)) / (2.0f * a);
			float t2 = (-b - sqrt(b2m4ac)) / (2.0f * a);
			t0 = min(t1, t2);
		}

		if (t0 <= 1.0f && t0 >= 0.0f)
		{
			interTime = t0;
			return 1;
		}

		return 0;
	}


	/******************************************************************************/
	/*!
		Handles the response when a circle collides with a lineSegment
		Reflects the circle off the lineSegment
	 */
	 /******************************************************************************/
	void CollisionResponse_CircleLineSegment(const Vector3D& ptInter,
		const Vector3D& normal,
		Vector3D& ptEnd,
		Vector3D& reflected)
	{
		// your code goes here
		//UNREFERENCED_PARAMETER(ptInter);
		//UNREFERENCED_PARAMETER(normal);
		//UNREFERENCED_PARAMETER(ptEnd);
		//UNREFERENCED_PARAMETER(reflected);

		/* penetration = BiBe */
		Vector3D penetration = ptEnd - ptInter;

		/* return Bi + penetration - 2(penetration . normal) * normal; */
		ptEnd = ptInter + penetration - 2 * VectorDot(penetration, normal) * normal;

		/* normalise BiBe = final velocity uv */
		VectorNorm(ptEnd - ptInter, reflected);

	}

	/******************************************************************************/
	/*!
		Handles the response when a circle collides with a pillar
	 */
	 /******************************************************************************/
	void CollisionResponse_CirclePillar(const Vector3D& normal, /* shld be normalised */
		const float& interTime,
		const Vector3D& ptStart,
		const Vector3D& ptInter,
		Vector3D& ptEnd,
		Vector3D& reflectedVectorNormalized)
	{
		UNREFERENCED_PARAMETER(interTime);
		/* BsBi vector of cirlce from start to intersection */
		Vector3D Bs_Bi = ptInter - ptStart;

		/* projection on to reflection normal */
		Vector3D proj_n = normal * VectorDot(Bs_Bi, normal);

		/* position after reflection */
		ptEnd = ptStart + ((Bs_Bi - proj_n) * 2.0f);

		/* new reflected vector */
		reflectedVectorNormalized = ptEnd - ptInter;
		VectorNorm(reflectedVectorNormalized, reflectedVectorNormalized);
	}

	/******************************************************************************/
	/*!
		Handles the response when a circle collides with another circle
		Extra credits
	 */
	 /******************************************************************************/
	void CollisionResponse_CircleCircle(Vector3D& normal,
		const float interTime,
		Vector3D& velA,
		const float& massA,
		Vector3D& interPtA,
		Vector3D& velB,
		const float& massB,
		Vector3D& interPtB,
		Vector3D& reflectedVectorA,
		Vector3D& ptEndA,
		Vector3D& reflectedVectorB,
		Vector3D& ptEndB)
	{
		Vector3D d = interPtA - interPtB;
		/* local coordinate d */
		Vector3D ud;
		VectorNorm(ud, d);
		/* perpendicular to d */
		Vector3D udp;
		udp.x = ud.y;
		udp.y = -ud.x;

		/* re-write velocities in terms of ud and udp */
		float a_a = VectorDot(velA, normal);
		float a_b = VectorDot(velB, normal);

		/* calc p, change in momentum */
		float changeInMomentum =
			(2.0f * massA * massB * (a_a - a_b)) /
			(massA + massB)
			;

		reflectedVectorA = velA - (changeInMomentum / massA) * ud;
		reflectedVectorB = velB + (changeInMomentum / massB) * ud;

		ptEndA = interPtA + (1.0f - interTime) * reflectedVectorA;
		ptEndB = interPtB + (1.0f - interTime) * reflectedVectorB;
	}

}