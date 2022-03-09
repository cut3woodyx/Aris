/******************************************************************************/
/*!
\file		ComponentPhysics.cpp
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

#include "ComponentPhysics.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "SystemPhysics.h"
#include "ForceManager.h"
#include "Math_CollisionSAT.h"

constexpr auto STRING_MASS = "mass";
constexpr auto STRING_POSITION_X = "positionX";
constexpr auto STRING_POSITION_Y = "positionY";
constexpr auto STRING_GRAVITY = "gravity";
constexpr auto STRING_VELOCITY_X = "velocityX";
constexpr auto STRING_VELOCITY_Y = "velocityY";
constexpr auto STRING_ACCELERATION_X = "accelerationX";
constexpr auto STRING_ACCELERATION_Y = "accelerationY";
constexpr auto STRING_IS_STATIC = "static";
namespace FwEngine
{
	ComponentPhysics::ComponentPhysics() : ComponentPhysics(nullptr)
	{
	}

	ComponentPhysics::ComponentPhysics(GameObject* parent) 
		: _previousPosition{},_isStatic { false }, _mass{ 1 },
		_velocity{ 0 }, _acceleration{ 0 }, _accumulatedForce{}, 
		_transformComponent{ nullptr }, _collisionComponent{ nullptr }, IComponent(parent),
		coefficient_of_friction_Static{ 1 }, coefficient_of_friction_Kinetic{4}
	{
	}


	ComponentPhysics::~ComponentPhysics()
	{
	}

	void ComponentPhysics::AddForce(const Vector3D& force)
	{
		_acceleration += force;
	}

	void ComponentPhysics::AddForce(DirectionalForce& force)
	{
		ListOfDirectionalForces.push_back(&force);
	}

	void ComponentPhysics::AddForce(RotationalForce& force)
	{
		ListOfRotationalForces.push_back(&force);
	}

	void ComponentPhysics::AddForce(MagneticForce force)
	{
		ListOfMagneticForces.push_back(force);
	}


	void ComponentPhysics::SetPosition(Vector3D pos)
	{
		_transformComponent->_currentPosition = pos;
	}

	void ComponentPhysics::SetVelocity(Vector3D vec)
	{
		_velocity = vec;
	}

	void ComponentPhysics::Init(ParamValueMap& paramValues)
	{
		//std::cout << "ComponentPhysics init" << std::endl;
		_isEnabled = true;

		//Loading Variables
		_mass = std::stof(GetFirst(paramValues, STRING_MASS));
		_transformComponent = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		_collisionComponent = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentCollision>()->at(_parentGameObject->GetIndex());

		//Loading Starting Velocity
		_velocity.x = std::stof(GetFirst(paramValues, STRING_VELOCITY_X));
		_velocity.y = std::stof(GetFirst(paramValues, STRING_VELOCITY_Y));
		_velocity.z = 0;
		//Loading Starting Accelaration
		_acceleration.x = std::stof(GetFirst(paramValues, STRING_ACCELERATION_X));
		_acceleration.y = std::stof(GetFirst(paramValues, STRING_ACCELERATION_Y));
		// Loading Is Static
		_isStatic = std::stoi(GetFirst(paramValues, STRING_IS_STATIC));
	}

	void ComponentPhysics::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentPhysics& source = reinterpret_cast<ComponentPhysics&>(sauce);

		_mass = source._mass;
		_transformComponent = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		_collisionComponent = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentCollision>()->at(_parentGameObject->GetIndex());

		_velocity = source._velocity;

		_acceleration = source._acceleration;

		_isStatic = source._isStatic;

	}

	void ComponentPhysics::Update(float dt)
	{
		//std::cout << "ComponentPhysics update" << std::endl;
		
		//Do not integrate static bodies
		if (_isStatic) return;

		Vector3D currentPosition = _transformComponent->_currentPosition;

		//Store prev position
		_previousPosition = currentPosition;

		//adding up all apllied Directional force
		auto beginD = ListOfDirectionalForces.begin();
		auto endD = ListOfDirectionalForces.end();

		_accumulatedForce = Vector3D{ 0,0,0 };

		while (beginD!=endD)
		{
			_accumulatedForce += (*beginD)->ApplyForce(dt);
			++beginD;
		}

		//adding up all apllied Magnetic force
		auto beginM = ListOfMagneticForces.begin();
		auto endM = ListOfMagneticForces.end();

		while (beginM != endM)
		{
			_accumulatedForce += beginM->ApplyForce(dt);
			if (beginM->checkValidity())
			{
				++beginM;
			}
			else
			{
				ListOfMagneticForces.erase(beginM++);
			}
			
		}

		

		//auto beginR = ListOfRotationalForces.begin();
		//auto endR = ListOfRotationalForces.end();
		//while (beginR != endR)
		//{
		//	if (*(beginR)->ApplyForce(*_collisionComponent, dt) == false)
		//	{
		//		ListOfRotationalForces.erase(beginR++);
		//	}
		//	else
		//	{
		//		++beginR;
		//	}

		//}
		//_accumulatedForce += PHYSICS->_gravity;
		Vector3D newAcceleration(0.0f, 0.0f, 0.0f);
		if(_mass != 0)
			newAcceleration = _accumulatedForce / _mass + PHYSICS->_gravity * _mass;// +_acceleration * dt;

		//Integrate the velocity
		_preVelocity = _velocity;
		_velocity = _velocity + newAcceleration *dt;
		//applying frictional force
		_velocity.x *= 0.7f;

		if (-10 < _velocity.x && _velocity.x < 10)
		{
			_velocity.x = 0;
		}
		//Clamp to velocity max for numerical stability
		/*if (FwMath::Vector3DDotProduct(_velocity, _velocity) > PHYSICS->_maxVelocitySq)
		{
			FwMath::Vector3DNormalize(_velocity, _velocity);
			_velocity = _velocity * PHYSICS->_maxVelocity;
		}*/

		//Integrate the position using Euler 
		currentPosition = currentPosition + _velocity*  dt; //acceleration term is small

		_transformComponent->_currentPosition = currentPosition;
	}

	void ComponentPhysics::Free()
	{
		ListOfDirectionalForces.clear();
		ListOfMagneticForces.clear();
		ListOfRotationalForces.clear();
		//std::cout << "ComponentPhysics free" << std::endl;
	}

	void ComponentPhysics::Destroy()
	{
		//std::cout << "ComponentPhysics destroy" << std::endl;
	}

	std::pair<std::string, ParamValueMap> ComponentPhysics::GetParams()
	{
		ParamValueMap params;
		params.emplace(STRING_MASS, std::to_string(_mass));
		params.emplace(STRING_IS_STATIC, std::to_string(_isStatic));

		return { STRING_COMPONENT_PHYSICS, params };
	}

}
