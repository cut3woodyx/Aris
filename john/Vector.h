/******************************************************************************/
/*!
\file		Vector.h
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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include <cmath>

namespace FwMath
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	typedef union Vector3D
	{
		struct
		{
			float x, y, z;
		};
	
		float m[3];
	
		// Constructors
		Vector3D();
		Vector3D(float set);
		Vector3D(float x, float y, float z);

		// Copy constructors
		Vector3D(const Vector3D& rhs);
	
		// Assignment operators
		Vector3D& operator += (const Vector3D& rhs);
		Vector3D& operator -= (const Vector3D& rhs);
		Vector3D& operator *= (float rhs);
		Vector3D& operator /= (float rhs);
		float& operator[](int i);
		// Unary operators
		Vector3D operator -() const;

		operator glm::vec2() const;
		operator glm::vec3() const;
		operator glm::vec4() const;
		
		float length();
	} Vector3D, Vec2, Point2D, Pt2;
	
	
	#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
	#pragma warning( default : 4201 )
	#endif
	
	// Binary operators
	Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator * (const Vector3D& lhs, float rhs);
	Vector3D operator * (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator * (float lhs, const Vector3D& rhs);
	Vector3D operator / (const Vector3D& lhs, float rhs);
	
	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void	Vector3DNormalize(Vector3D& pResult, const Vector3D& pVec0);
	
	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vector3DLength(const Vector3D& pVec0);
	
	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareLength(const Vector3D& pVec0);
	
	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	 */
	 /**************************************************************************/
	float	Vector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1);
	
	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1);
	
	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1);
	
	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DCrossProductMag(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		This function projects Vector1 onto Vector0
	*/
	/**************************************************************************/
	Vector3D	Vector3DProjection(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		This function tells if two vectors are intersecting
	*/
	/**************************************************************************/
	bool	Vector3DIntersection(
		const Vector3D& point0, const Vector3D& Vec0,
		const Vector3D& point1, const Vector3D& Vec1);

	/**************************************************************************/
	/*!
		fast inverse sqrt for normalizing
	*/
	/**************************************************************************/
	float inverseSqrt(float);
}
	
	