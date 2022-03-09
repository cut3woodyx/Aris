/******************************************************************************/
/*!
\file		ComponentCollision.cpp
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
#include <iostream>
#include "Component.h"
#include "ComponentCollision.h"
#include "SystemManager.h"
constexpr auto STRING_vertexA_X = "vertexA_X";
constexpr auto STRING_vertexA_Y = "vertexA_Y";
constexpr auto STRING_vertexB_X = "vertexB_X";
constexpr auto STRING_vertexB_Y = "vertexB_Y";
constexpr auto STRING_vertexC_X = "vertexC_X";
constexpr auto STRING_vertexC_Y = "vertexC_Y";
constexpr auto STRING_vertexD_X = "vertexD_X";
constexpr auto STRING_vertexD_Y = "vertexD_Y";

constexpr auto STRING_AABB = "AABB";
constexpr auto STRING_SAT = "SAT";

namespace FwEngine
{


	ComponentCollision::ComponentCollision() :
		_vertexA{ -0.5f,-0.5f, 0.0f }, _vertexB{ 0.5f,0.5f,0.0f },
		_vertexC{ 0.5f, -0.5f, 0.0f }, _vertexD{ -0.5f,0.5f,0.0f }, collisionEvent(OBJ_MAX),
		collisionFlag{ 0 }, previousCollisionFlag{ 0 }, isAABB{ false }, isSAT{ false }
	{
		for ( bool i : collisionEvent)
		{
			i = false;
		}
	}

	ComponentCollision::ComponentCollision(GameObject* parent) : IComponent(parent),
		_vertexA{ -0.5f,-0.5f,0.0f }, _vertexB{ 0.5f,0.5f,0.0f },
		_vertexC{ 0.5f, -0.5f, 0.0f }, _vertexD{ -0.5f,0.5f,0.0f }, collisionEvent(OBJ_MAX),
		collisionFlag{ 0 }, previousCollisionFlag{ 0 },  isAABB{ false }, isSAT{ false }
	{
		for (bool i : collisionEvent)
		{
			i = false;
		}
	}

	ComponentCollision::~ComponentCollision()
	{
	}

	bool ComponentCollision::isCollidingWith(std::string tag)
	{
		for (size_t i = 0; i < OBJ_MAX; i++)
		{
			if (collisionEvent[i])
			{
				if (this->getParentGameObject()->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(i)._tag == tag)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool ComponentCollision::isCollidingWithByName(std::string name)
	{
		for (size_t i = 0; i < OBJ_MAX; i++)
		{
			if (collisionEvent[i])
			{
				std::string objName = this->getParentGameObject()->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(i).getParentGameObject()->GetName();
				if (objName == name)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool ComponentCollision::checkInAir()
	{
		bool check = true;
		auto begin = Collision_Check_List.begin();
		auto end = Collision_Check_List.end();
		while (begin != end)
		{
			if (begin->first != "blast" && 
				begin->first.find("Dialogue") == std::string::npos &&
				begin->first != "")
			{
				check = false;
			}
			++begin;
		}
		
		return check;
	}

	bool ComponentCollision::checkLanding()
	{
		bool check = false;
		bool pcheck = false;

		auto begin = Collision_Check_List.begin();
		auto end = Collision_Check_List.end();
		auto pbegin = Prev_Collision_Check_List.begin();
		auto pend = Prev_Collision_Check_List.end();
		while(begin!=end)
		{
			if (begin->first == "")
			{
				++begin;
				continue;
			}
			if (begin->second == 'B')
			{
				check = true;
			}
			++begin;
		}
		while (pbegin!=pend)
		{
			if (pbegin->first == "")
			{
				++pbegin;
				continue;
			}
			if (pbegin->second == 'B')
			{
				pcheck = true;
			}
			++pbegin;
		}

		if (check == true && pcheck == false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	char ComponentCollision::prevCheckCollisionWith(std::string tag)
	{
		auto iterator = Prev_Collision_Check_List.find(tag);
		if (iterator != Prev_Collision_Check_List.end())
		{
			return iterator->second;
		}
		else
		{
			return 0;
		}
	}

	//returns the which side the object is colliding with tag
	char ComponentCollision::checkCollisionWith(std::string tag)
	{
		auto iterator = Collision_Check_List.find(tag);
		if (iterator != Collision_Check_List.end())
		{
			return iterator->second;
		}
		else
		{
			return 0;
		}
	}

	//bool ComponentCollision::OnCollisionEnter()
	//{
	//	if ((_collision_last == false) && (_collision_now == true))
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	//bool ComponentCollision::OnCollisionStay()
	//{
	//	if (_collision_last && _collision_now)
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	//bool ComponentCollision::OnCollisionLeave()
	//{
	//	if (_collision_last && (_collision_now == false))
	//	{
	//		return true;
	//	}
	//	return false;
	//}


	void ComponentCollision::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		//serializing
		float vertexA_X = std::stof(GetFirst(paramValues, STRING_vertexA_X));
		float vertexA_Y = std::stof(GetFirst(paramValues, STRING_vertexA_Y));
		float vertexB_X = std::stof(GetFirst(paramValues, STRING_vertexB_X));
		float vertexB_Y = std::stof(GetFirst(paramValues, STRING_vertexB_Y));

		isAABB = std::stof(GetFirst(paramValues, STRING_vertexB_Y));
		isSAT = std::stof(GetFirst(paramValues, STRING_vertexB_Y));

		_vertexA = FwMath::Vector3D(vertexA_X, vertexA_Y, 0);
		_vertexB = FwMath::Vector3D(vertexB_X, vertexB_Y, 0);
		_vertexC = FwMath::Vector3D(vertexB_X, vertexA_Y, 0);
		_vertexD = FwMath::Vector3D(vertexA_X, vertexB_Y, 0);
	}

	void ComponentCollision::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentCollision& source = reinterpret_cast<ComponentCollision&>(sauce);

		isAABB = source.isAABB;
		isSAT = source.isSAT;

		_vertexA = source._vertexA;
		_vertexB = source._vertexB;
		_vertexC = source._vertexC;
		_vertexD = source._vertexD;

	}

	void ComponentCollision::Update(float )
	{
		//std::cout << "ComponentCollision update" << std::endl;
	}

	void ComponentCollision::Free()
	{
		//std::cout << "ComponentCollision free" << std::endl;
	}

	void ComponentCollision::Destroy()
	{
		//std::cout << "ComponentCollision destroy" << std::endl;
	}

	std::pair<std::string, ParamValueMap> ComponentCollision::GetParams()
	{
		ParamValueMap params;
		params.emplace(STRING_vertexA_X, std::to_string(_vertexA.x));
		params.emplace(STRING_vertexA_Y, std::to_string(_vertexA.y));

		params.emplace(STRING_vertexB_X, std::to_string(_vertexB.x));
		params.emplace(STRING_vertexB_Y, std::to_string(_vertexB.y));

		return { STRING_COMPONENT_COLLISION, params };
	}

	//ComponentCollision::Shape::Shape(ShapeId pid) : Id(pid)
	//{

	//}

}

