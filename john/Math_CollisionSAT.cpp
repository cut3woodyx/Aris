/******************************************************************************/
/*!
\file		Math_CollisionSAT.cpp
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
#include "Math_CollisionSAT.h"
#include <vector>
#include <iostream>
#include <limits.h>
namespace FwMath
{
	bool CollisionIntersection_SAT_projection(
		const FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1,
		const FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2)
	{
		//store projection of axis2 to axis 1
		Vector3D projVector;
		Vector3D projStartingPoint;
		//two vector for compairison check
		Vector3D compairVector1;
		Vector3D compairVector2;

		//calculating vertex in global position for obj1
		Vector3D vertexA1 = pos1._currentPosition + obj1._vertexA * pos1._scale;
		Vector3D vertexB1 = pos1._currentPosition + obj1._vertexB * pos1._scale;
		Vector3D vertexC1 = pos1._currentPosition + obj1._vertexC * pos1._scale;
		Vector3D vertexD1 = pos1._currentPosition + obj1._vertexD * pos1._scale;

		//calculating vertex in global position for obj2
		Vector3D vertexA2 = pos2._currentPosition + obj2._vertexA * pos2._scale;
		Vector3D vertexB2 = pos2._currentPosition + obj2._vertexB * pos2._scale;
		Vector3D vertexC2 = pos2._currentPosition + obj2._vertexC * pos2._scale;
		Vector3D vertexD2 = pos2._currentPosition + obj2._vertexD * pos2._scale;

		//usefull data for shape 1
		std::vector<Vector3D> axisTable1(4);
		axisTable1[0] = vertexA1 - vertexC1;
		axisTable1[1] = vertexC1 - vertexB1;
		axisTable1[2] = vertexB1 - vertexD1;
		axisTable1[3] = vertexD1 - vertexA1;

		std::vector<Vector3D> startPointTable1(4);
		startPointTable1[0] = vertexC1;
		startPointTable1[1] = vertexB1;
		startPointTable1[2] = vertexD1;
		startPointTable1[3] = vertexA1;

		//usefull data for shape 2
		std::vector<Vector3D> axisTable2(4);
		axisTable2[0] = vertexA2 - vertexC2;
		axisTable2[1] = vertexC2 - vertexB2;
		axisTable2[2] = vertexB2 - vertexD2;
		axisTable2[3] = vertexD2 - vertexA2;

		std::vector<Vector3D> startPointTable2(4);
		startPointTable2[0] = vertexC2;
		startPointTable2[1] = vertexB2;
		startPointTable2[2] = vertexD2;
		startPointTable2[3] = vertexA2;

		for (int i = 0; i < 4; i++)
		{
			Vector3D projectedShapeLowestPoint{ 99999999.0f };
			Vector3D projectedShapeHighestPoint{ -99999999.0f };
			for (int j = 0; j < 4; j++)
			{
				//project axis 2 onto axis 1
				projVector = Vector3DProjection(axisTable1[i], axisTable2[j]);
				//project starting point of obj2 onto obj1
				projStartingPoint = startPointTable1[i] + Vector3DProjection(axisTable1[i], startPointTable1[i] - startPointTable2[j]);

				//calculating projected shape lowest point
				if ((projStartingPoint.x < projectedShapeLowestPoint.x)
					|| (projStartingPoint.y < projectedShapeLowestPoint.y))
				{
					projectedShapeLowestPoint = projStartingPoint;
				}

				if (((projStartingPoint.x + projVector.x) < projectedShapeLowestPoint.x)
					|| ((projStartingPoint.y + projVector.y) < projectedShapeLowestPoint.y))
				{
					projectedShapeLowestPoint = projStartingPoint + projVector;
				}

				//calculating projected shape highest point
				if ((projStartingPoint.x > projectedShapeHighestPoint.x)
					|| (projStartingPoint.y > projectedShapeHighestPoint.y))
				{
					projectedShapeHighestPoint = projStartingPoint;
				}

				if (((projStartingPoint.x + projVector.x) > projectedShapeHighestPoint.x)
					|| ((projStartingPoint.y + projVector.y) > projectedShapeHighestPoint.y))
				{
					projectedShapeHighestPoint = projStartingPoint + projVector;
				}
			}
			if (!(Vector3DIntersection(startPointTable1[i], axisTable1[i], projectedShapeLowestPoint, projectedShapeHighestPoint - projectedShapeLowestPoint)))
			{
				return false;
			}

		}

		return true;
	}

	bool CollisionIntersection_SAT_DotProduct(
		const FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1,
		const FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2)
	{
		//calculating vertex in global position for obj1
		//0 is the btm left, going in clockwise direction
		std::vector<Vector3D> VertexTable1(4);
		VertexTable1[0] = pos1._currentPosition + obj1._vertexA * pos1._scale;
		VertexTable1[1] = pos1._currentPosition + obj1._vertexC * pos1._scale;
		VertexTable1[2] = pos1._currentPosition + obj1._vertexB * pos1._scale;
		VertexTable1[3] = pos1._currentPosition + obj1._vertexD * pos1._scale;

		//calculating vertex in global position for obj2
		//0 is the btm left, going in clockwise direction
		std::vector<Vector3D> VertexTable2(4);
		VertexTable2[0] = pos2._currentPosition + obj2._vertexA * pos2._scale;
		VertexTable2[1] = pos2._currentPosition + obj2._vertexC * pos2._scale;
		VertexTable2[2] = pos2._currentPosition + obj2._vertexB * pos2._scale;
		VertexTable2[3] = pos2._currentPosition + obj2._vertexD * pos2._scale;

		//usefull data for shape 1
		std::vector<Vector3D> axisTable1(4);
		axisTable1[0] = VertexTable1[0] - VertexTable1[1];
		axisTable1[1] = VertexTable1[1] - VertexTable1[2];
		axisTable1[2] = VertexTable1[2] - VertexTable1[3];
		axisTable1[3] = VertexTable1[3] - VertexTable1[1];

		//usefull data for shape 2
		std::vector<Vector3D> axisTable2(4);
		axisTable2[0] = VertexTable2[0] - VertexTable2[1];
		axisTable2[1] = VertexTable2[1] - VertexTable2[2];
		axisTable2[2] = VertexTable2[2] - VertexTable2[3];
		axisTable2[3] = VertexTable2[3] - VertexTable2[1];

		std::vector<Vector3D> normalTable1(4);
		std::vector<Vector3D> normalTable2(4);
		for (size_t i = 0; i < 4; i++)
		{
			normalTable1[i].x = -axisTable1[i].y;
			normalTable1[i].y = axisTable1[i].x;

			normalTable2[i].x = -axisTable2[i].y;
			normalTable2[i].y = axisTable2[i].x;
		}

		//check for first shape with second shape
		for (size_t i = 0; i < 4; i++)
		{
			size_t j;
			for (j = 0; j < 4; j++)
			{
				//if the dot product is positive, no intersection
				if (Vector3DDotProduct(VertexTable2[i] - VertexTable1[j], normalTable1[j]) > 0)
				{
					break;
				}
			}
			if (j == 4)
			{
				return true;
			}
		}
		//check for second shape with first shape
		for (size_t i = 0; i < 4; i++)
		{
			size_t j;
			for (j = 0; j < 4; j++)
			{
				//if the dot product is positive, no intersection
				if (Vector3DDotProduct(VertexTable1[i] - VertexTable2[j], normalTable2[j]) > 0)
				{
					break;
				}
			}
			if (j == 4)
			{
				return true;
			}
		}


		return false;
	}
	bool CollisionIntersection_SAT_Intersection(const FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1, const FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2)
	{
		//calculating vertex in global position for obj1
		Vector3D vertexA1 = pos1._currentPosition + obj1._vertexA * pos1._scale;
		Vector3D vertexB1 = pos1._currentPosition + obj1._vertexB * pos1._scale;
		Vector3D vertexC1 = pos1._currentPosition + obj1._vertexC * pos1._scale;
		Vector3D vertexD1 = pos1._currentPosition + obj1._vertexD * pos1._scale;

		//calculating vertex in global position for obj2
		Vector3D vertexA2 = pos2._currentPosition + obj2._vertexA * pos2._scale;
		Vector3D vertexB2 = pos2._currentPosition + obj2._vertexB * pos2._scale;
		Vector3D vertexC2 = pos2._currentPosition + obj2._vertexC * pos2._scale;
		Vector3D vertexD2 = pos2._currentPosition + obj2._vertexD * pos2._scale;

		//usefull data for shape 1
		std::vector<Vector3D> axisTable1(4);
		axisTable1[0] = vertexA1 - vertexC1;
		axisTable1[1] = vertexC1 - vertexB1;
		axisTable1[2] = vertexB1 - vertexD1;
		axisTable1[3] = vertexD1 - vertexA1;

		std::vector<Vector3D> startPointTable1(4);
		startPointTable1[0] = vertexC1;
		startPointTable1[1] = vertexB1;
		startPointTable1[2] = vertexD1;
		startPointTable1[3] = vertexA1;

		//usefull data for shape 2
		std::vector<Vector3D> axisTable2(4);
		axisTable2[0] = vertexA2 - vertexC2;
		axisTable2[1] = vertexC2 - vertexB2;
		axisTable2[2] = vertexB2 - vertexD2;
		axisTable2[3] = vertexD2 - vertexA2;

		std::vector<Vector3D> startPointTable2(4);
		startPointTable2[0] = vertexC2;
		startPointTable2[1] = vertexB2;
		startPointTable2[2] = vertexD2;
		startPointTable2[3] = vertexA2;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				if (Vector3DIntersection(startPointTable1[i],axisTable1[i],startPointTable2[j],axisTable2[j]))
				{
					return true;
				}
			}
		}
		return false;
	}
	void checkCollisionSides(
		FwEngine::ComponentCollision& obj1, const FwEngine::ComponentTransform& pos1, 
		FwEngine::ComponentCollision& obj2, const FwEngine::ComponentTransform& pos2, 
		int& flag)
	{
		Vector3D Obj1VertexMax = pos1._currentPosition + obj1._vertexB * pos1._scale;
		Vector3D Obj1VertexMin = pos1._currentPosition + obj1._vertexA * pos1._scale;
		Vector3D Obj1AbosoluteVertex = (Obj1VertexMax - Obj1VertexMin) / 2;

		Vector3D Obj2VertexMax = pos2._currentPosition + obj2._vertexB * pos2._scale;
		Vector3D Obj2VertexMin = pos2._currentPosition + obj2._vertexA * pos2._scale;
		Vector3D Obj2AbosoluteVertex = (Obj2VertexMax - Obj2VertexMin) / 2;
		
		Vector3D Obj1AbsolutePosition = (Obj1VertexMax + Obj1VertexMin) / 2;
		Vector3D Obj2AbsolutePosition = (Obj2VertexMax + Obj2VertexMin) / 2;

		Vector3D relativeDistance = Obj2AbsolutePosition - Obj1AbsolutePosition;
		Vector3D DistanceCheck = relativeDistance;

		relativeDistance.x = fabsf(relativeDistance.x);
		relativeDistance.y = fabsf(relativeDistance.y);

		Vector3D absoluteDistance = Obj1AbosoluteVertex + Obj2AbosoluteVertex;
		Vector3D penetration = absoluteDistance - relativeDistance;

		//std::cout << "Penetration x : " << penetration.x << "Penetration y : " << penetration.y << std::endl;

		if (penetration.x > penetration.y)
		{
			if (DistanceCheck.y >= 0)
			{
				//btm
				flag = flag | COLLISION_TOP;
				obj1.Collision_Check_List.insert(std::pair<std::string,char>(pos2._tag,'T'));
			}
			if (DistanceCheck.y <= 0)
			{
				//top
				flag = flag | COLLISION_BOTTOM;
				obj1.Collision_Check_List.insert(std::pair<std::string, char>(pos2._tag, 'B'));
			}
		}
		else
		{
			if (DistanceCheck.x >= 0)
			{
				//right
				flag = flag | COLLISION_RIGHT;
				obj1.Collision_Check_List.insert(std::pair<std::string, char>(pos2._tag, 'R'));
			}
			if (DistanceCheck.x <= 0)
			{
				//left
				flag = flag | COLLISION_LEFT;
				obj1.Collision_Check_List.insert(std::pair<std::string, char>(pos2._tag, 'L'));
			}
		}
	}
}