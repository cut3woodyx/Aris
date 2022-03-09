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
		: _previousPosition{},_isStatic { false }, _mass{ 0 },
		_velocity{ 0 }, _acceleration{ 0 }, _accumulatedForce{}, 
		_transformComponent{ nullptr }, _collisionComponent{ nullptr }, IComponent(parent)
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
		Vector3D newAcceleration = _accumulatedForce / _mass + PHYSICS->_gravity;// +_acceleration * dt;
		//Integrate the velocity
		_velocity = _velocity + newAcceleration *dt;


		//Clamp to velocity max for numerical stability
		/*if (FwMath::Vector3DDotProduct(_velocity, _velocity) > PHYSICS->_maxVelocitySq)
		{
			FwMath::Vector3DNormalize(_velocity, _velocity);
			_velocity = _velocity * PHYSICS->_maxVelocity;
		}*/

		//Clear the force


		//Integrate the position using Euler 
		currentPosition = currentPosition + _velocity*  dt; //acceleration term is small

		_transformComponent->_currentPosition = currentPosition;
	}

	void ComponentPhysics::Free()
	{
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
