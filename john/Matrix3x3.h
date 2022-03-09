/******************************************************************************/
/*!
\file		Matrix3x3.h
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
#include "Vector.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace FwMath
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

/**************************************************************************/
/*!
	An abstraction of 3x3 matrix
 */
 /**************************************************************************/
	typedef union Matrix3x3
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		float m[16];
		float m2[4][4];

		Matrix3x3();
		Matrix3x3(const float x);
		Matrix3x3(const float* pArr);
		Matrix3x3(	float _00, float _01, float _02, float _03,
					float _10, float _11, float _12, float _13,
					float _20, float _21, float _22, float _23,
					float _30, float _31, float _32, float _33);
		Matrix3x3& operator=(const Matrix3x3& rhs);

		// Assignment operators
		Matrix3x3& operator *= (const Matrix3x3& rhs);

		float& operator[](int i);
		operator glm::mat4x4() const;
		operator glm::mat3x3() const;

	} Matrix3x3, Matrix3x3, Mtx44;

	Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs);

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	 */
	 /**************************************************************************/
	Vector3D  operator * (const Matrix3x3& pMtx, const Vector3D& rhs);

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
	 */
	 /**************************************************************************/
	void Mtx33Identity(Matrix3x3& pResult);

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void Mtx33Translate(Matrix3x3& pResult, float x, float y);

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void Mtx33Scale(Matrix3x3& pResult, float x, float y);

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx33RotRad(Matrix3x3& pResult, float angle);

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx33RotDeg(Matrix3x3& pResult, float angle);

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
	 */
	 /**************************************************************************/
	void Mtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx);

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx33Inverse(Matrix3x3* pResult, float* determinant, const Matrix3x3& pMtx);
}