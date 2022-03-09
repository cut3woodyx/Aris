/******************************************************************************/
/*!
\file		Math_Collision.h
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

//using CS230::Vector3D;
//using CS230::Matrix3x3;

namespace FwMath
{
	/******************************************************************************/
	/*!
		reprensents a straight line segment with 2 vertices and 1 normal
	 */
	 /******************************************************************************/
	struct LineSegment
	{
		Vector3D	m_pt0;
		Vector3D	m_pt1;
		Vector3D	m_normal;
	};

	void BuildLineSegment(LineSegment& lineSegment,								//Line segment reference - input
		const Vector3D& pos,										//position - input
		float scale,											//scale - input
		float dir);											//direction - input

/******************************************************************************/
/*!
	represents a circle
 */
 /******************************************************************************/
	struct Circle
	{
		Vector3D  m_center;
		float	m_radius;

		// Extra credits
		float   m_mass{ 1.0f };
	};

	struct Ray
	{
		Vector3D	m_pt0;
		Vector3D	m_dir;
	};


	// INTERSECTION FUNCTIONS
	int CollisionIntersection_CircleLineSegment(const Circle& circle,			//Circle data - input
		const Vector3D& ptEnd,														//End circle position - input
		const LineSegment& lineSeg,												//Line segment - input
		Vector3D& interPt,															//Intersection position of the circle - output 
		Vector3D& normalAtCollision,												//Normal vector at collision time - output
		float& interTime,														//Intersection time ti - output
		bool& checkLineEdges);													//The last parameter is new - for Extra Credits: true = check collision with line segment edges

	// Extra credits
	int CheckMovingCircleToLineEdge(bool withinBothLines,						//Flag stating that the circle is starting from between 2 imaginary line segments distant +/- Radius respectively - input
		const Circle& circle,													//Circle data - input
		const Vector3D& ptEnd,														//End circle position - input
		const LineSegment& lineSeg,												//Line segment - input
		Vector3D& interPt,															//Intersection position of the circle - output 
		Vector3D& normalAtCollision,												//Normal vector at collision time - output
		float& interTime);														//Intersection time ti - output


	// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
	// In the case of "dynamic circle vs static pillar", velB will be 0.0f
	int CollisionIntersection_CircleCircle(const Circle& circleA,				//CircleA data - input
		const Vector3D& velA,														//CircleA velocity - input
		const Circle& circleB,													//CircleB data - input
		const Vector3D& velB,														//CircleA velocity - input
		Vector3D& interPtA,															//Intersection point of CircleA at collision time - output
		Vector3D& interPtB,															//Intersection point of CircleB at collision time - output
		float& interTime);														//intersection time - output

	// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
	// this is a helper function to be used inside "CollisionIntersection_CircleCircle" function
	// it is checking collision a moving dot vs a static circle
	int CollisionIntersection_RayCircle(const Ray& ray,							//A ray containing the data of the moving dot - input
		const Circle& circle,													//Static circle data - input
		float& interTime);														//Intersection time - output






	// RESPONSE FUNCTIONS
	void CollisionResponse_CircleLineSegment(const Vector3D& ptInter,				//Intersection position of the circle - input
		const Vector3D& normal,														//Normal vector of reflection on collision time - input
		Vector3D& ptEnd,															//Final position of the circle after reflection - output
		Vector3D& reflected);														//Normalized reflection vector direction - output

	void CollisionResponse_CirclePillar(const Vector3D& normal,						//Normal vector of reflection on collision time - input
		const float& interTime,													//Intersection time - input
		const Vector3D& ptStart,													//Starting position of the circle - input
		const Vector3D& ptInter,													//Intersection position of the circle - input
		Vector3D& ptEnd,															//Final position of the circle after reflection - output
		Vector3D& reflectedVectorNormalized);										//Normalized reflection vector - output

	// Extra credits
	void CollisionResponse_CircleCircle(Vector3D& normal,							//Normal vector of reflection on collision time - input
		const float interTime,													//Intersection time - input
		Vector3D& velA,																//Velocity of CircleA - input
		const float& massA,														//Mass of CircleA - input
		Vector3D& interPtA,															//Intersection position of circle A at collision time - input
		Vector3D& velB,																//Velocity of CircleB - input
		const float& massB,														//Mass of CircleB - input
		Vector3D& interPtB,															//Intersection position of circle B at collision time - input
		Vector3D& reflectedVectorA,													//Non-Normalized reflected vector of Circle A - output
		Vector3D& ptEndA,															//Final position of the circle A after reflection - output
		Vector3D& reflectedVectorB,													//Non-Normalized reflected vector of Circle B - output
		Vector3D& ptEndB);															//Final position of the circle B after reflection - output

}
