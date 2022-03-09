/******************************************************************************/
/*!
\file		ComponentCollision.h
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

#include "IComponent.h"
#include "ComponentPhysics.h"
#include "Math.h"
#include <vector>
#include <map>
namespace FwEngine
{
	/*class ComponentPhysics;*/
	constexpr auto STRING_COMPONENT_COLLISION = "collision";
	class ComponentCollision : public IComponent
	{
	public:
		//class ComponentPhysics;
		int collisionFlag;
		int previousCollisionFlag;
		bool isAABB;
		bool isSAT;
		FwMath::Vector3D _vertexA;
		FwMath::Vector3D _vertexB;
		FwMath::Vector3D _vertexC;
		FwMath::Vector3D _vertexD;
		std::vector<bool> collisionEvent;
		FwMath::Vector3D _collision_normal;

		std::unordered_map<std::string, char> Collision_Check_List;
		std::unordered_map<std::string, char> Prev_Collision_Check_List;
		

		ComponentCollision();
		ComponentCollision(GameObject* parent);
		~ComponentCollision();

		bool isCollidingWith(std::string tag);
		bool isCollidingWithByName(std::string tag);
		bool checkInAir();
		bool checkLanding();
		char prevCheckCollisionWith(std::string tag);
		char checkCollisionWith(std::string tag);
		
		//bool OnCollisionEnter();
		//bool OnCollisionStay();
		//bool OnCollisionLeave();

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};
}
