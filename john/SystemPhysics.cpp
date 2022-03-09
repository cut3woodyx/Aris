#include "WindowSystem.h"
#include <iostream>
#include "SystemPhysics.h"
#include "SystemInput.h"
#include "ComponentScript.h"
#include "Math_CollisionSAT.h"
#include "Map.h"
namespace FwEngine
{
	SystemPhysics * PHYSICS = nullptr;

	bool SystemPhysics::DoCollisionCheck(
		const ComponentPhysics* componentPhysics1,
		const FwMath::AABB colComp1AABB,
		const ComponentPhysics* componentPhysics2,
		const FwMath::AABB colComp2AABB,
		const float dt,
		Vector3D& normal)
	{

		return FwMath::CollisionIntersection_RectRect
		(	
			colComp1AABB,
			componentPhysics1->_velocity,
			colComp2AABB,
			componentPhysics2->_velocity,
			dt,
			normal
		);
	}

	void SystemPhysics::DoCollisionResponse(
		ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1,
		ComponentPhysics* phys2, ComponentTransform* trans2, ComponentCollision* coll2)
	{

		if (trans1->_tag == "metal" && trans2->_tag == "metal")
		{
			StackingResponse(phys1, trans1, coll1, phys2, trans2, coll2);
		}
		
	}

	SystemPhysics::SystemPhysics() : SystemPhysics(nullptr)
	{}

	SystemPhysics::SystemPhysics(GameObjectPool* gameObjectPool)
		: ISystem(gameObjectPool), _gravity{ 0, -500, 0 }, _maxVelocity{ 500 },
		_maxVelocitySq{ _maxVelocity * _maxVelocity }, _penetrationEpsilon{ 0.2f }, _penetrationResolvePercentage{ 0.8f },
		_map(nullptr), _runCollision{ true }, _listener{ "PhysicsListener" }
	{
		PHYSICS = this;
	}


	SystemPhysics::~SystemPhysics()
	{
	}

	void SystemPhysics::SetMap(Map* map)
	{
		_map = map;
	}

	void SystemPhysics::Init()
	{
		//std::cout << "SystemPhysics init" << std::endl;
	}

	void SystemPhysics::Update(float dt)
	{
		//std::cout << "SystemPhysics update" << std::endl;

		ComponentPhysics* phyComp1 = &_gameObjectPool->GetContainerPtr<ComponentPhysics>()->at(0);
		ComponentCollision* colComp1 = &_gameObjectPool->GetContainerPtr<ComponentCollision>()->at(0);
		ComponentTransform* trans1 = &_gameObjectPool->GetContainerPtr<ComponentTransform>()->at(0);
		GameObject* gameObject1 = _gameObjectPool->GetGameObject(0);

		
		// INPUT TESTING
		//SystemInput* inp = windowsMain->_input;
		//if ((*inp)(IKEY_W, SystemInput::HOLD))
		//{
		//	if (phyComp1->isEnabled())
		//		phyComp1->AddForce({0,100,0});
		//}
		//if ((*inp)(IKEY_A, SystemInput::HOLD))
		//{
		//	if (phyComp1->isEnabled())
		//		phyComp1->AddForce({ -100,0,0 });
		//}
		//if ((*inp)(IKEY_S, SystemInput::HOLD))
		//{
		//	if (phyComp1->isEnabled())
		//		phyComp1->AddForce({ 0,-100,0 });
		//}
		//if ((*inp)(IKEY_D, SystemInput::HOLD))
		//{
		//	if (phyComp1->isEnabled())
		//		phyComp1->AddForce({ 100,0,0 });
		//}

		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentCollision>()->size();
		for (size_t i = 0; i < compSize; ++phyComp1, ++colComp1, ++gameObject1, ++trans1, ++i)
		{
			
			// dun check inactives
			if (!gameObject1->_isActive)
				continue;

			if (phyComp1->isEnabled())
				phyComp1->Update(dt);

			if (colComp1->isEnabled())
			{
				colComp1->previousCollisionFlag = colComp1->collisionFlag;
				if (phyComp1->isEnabled())
				{

					//float scaleX = abs(colComp1->vertexA.x) + abs(colComp1->vertexB.x);
					//float scaleY = abs(colComp1->vertexA.y) + abs(colComp1->vertexB.y);

					int flag = _map->CheckCellCollision(
						trans1->_currentPosition.x, trans1->_currentPosition.y,
						abs(trans1->_scale.x * colComp1->_vertexA.x),
						abs(trans1->_scale.x * colComp1->_vertexB.x),
						abs(trans1->_scale.y * colComp1->_vertexA.y),
						abs(trans1->_scale.y * colComp1->_vertexB.y)
					);
					colComp1->collisionFlag = flag;

					if (flag & _map->COLLISION_TOP)
					{
						float newY = _map->SnapToCell(phyComp1->_previousPosition.y,
							abs(trans1->_scale.y * colComp1->_vertexB.y),
							_map->COLLISION_TOP);
						phyComp1->SetVelocity({ phyComp1->_velocity.x, 0, phyComp1->_velocity.z });
						phyComp1->SetPosition({ trans1->_currentPosition.x, newY, trans1->_currentPosition.z });
					}
					if (flag & _map->COLLISION_BOTTOM)
					{
						float newY = _map->SnapToCell(phyComp1->_previousPosition.y,
							abs(trans1->_scale.y * colComp1->_vertexA.y),
							_map->COLLISION_BOTTOM);
						phyComp1->SetVelocity({ phyComp1->_velocity.x, 0, phyComp1->_velocity.z });
						phyComp1->SetPosition({ trans1->_currentPosition.x, newY, trans1->_currentPosition.z });
					}
					if (flag & _map->COLLISION_LEFT)
					{
						float newX = _map->SnapToCell(phyComp1->_previousPosition.x,
							abs(trans1->_scale.x * colComp1->_vertexA.x),
							_map->COLLISION_LEFT);
						phyComp1->SetVelocity({ 0, phyComp1->_velocity.y, phyComp1->_velocity.z });
						phyComp1->SetPosition({ newX, trans1->_currentPosition.y, trans1->_currentPosition.z });
					}
					if (flag & _map->COLLISION_RIGHT)
					{
						float newX = _map->SnapToCell(phyComp1->_previousPosition.x,
							abs(trans1->_scale.x * colComp1->_vertexB.x),
							_map->COLLISION_RIGHT);
						phyComp1->SetVelocity({ 0, phyComp1->_velocity.y, phyComp1->_velocity.z });
						phyComp1->SetPosition({ newX, trans1->_currentPosition.y, trans1->_currentPosition.z });
					}
				}

				if (_runCollision && (i < (compSize - 1)))
				{
					ComponentCollision* colComp2 = &_gameObjectPool->GetContainerPtr<ComponentCollision>()->at(i + 1);
					GameObject* gameObject2 = _gameObjectPool->GetGameObject(i + 1);
					// warning will result in errors if transform component is not init (!)
					ComponentPhysics* phyComp2 = &_gameObjectPool->GetContainerPtr<ComponentPhysics>()->at(i + 1); 
					ComponentTransform* trans2 = &_gameObjectPool->GetContainerPtr<ComponentTransform>()->at(i + 1);

					for (size_t j = i + 1; j < compSize; ++colComp2, ++gameObject2, ++phyComp2, ++trans2, ++j)
					{
						//if (!_runCollision)
						//	continue;
						// do collision check btw colComp1(gameObject1) and colComp2(gameObject2)

						// dun check inactives or if same
						if (!gameObject2->_isActive
							|| !colComp2->isEnabled())
							continue;

						bool sat = false;
						bool aabb = false;
						//update collider
						if (trans1->_tag=="blast"||trans2->_tag=="blast")
						{
							sat = FwMath::CollisionIntersection_SAT_Intersection(*colComp1, *trans1, *colComp2, *trans2);
						}
						else
						{
							aabb = DoCollisionCheck(phyComp1,
								{ colComp1->_vertexA * trans1->_scale + trans1->_currentPosition ,
								colComp1->_vertexB * trans1->_scale + trans1->_currentPosition },
								phyComp2,
								{ colComp2->_vertexA * trans2->_scale + trans2->_currentPosition ,
								colComp2->_vertexB * trans2->_scale + trans2->_currentPosition },
								dt,
								colComp1->_collision_normal);

							if (aabb)
							{
								FwMath::checkCollisionSides(*colComp1,*trans1,*colComp2,*trans2,colComp1->collisionFlag);

							}
						}

						

						if (aabb || sat)
						//if (FwMath::CollisionIntersection_SAT_DotProduct(*colComp1, *trans1, *colComp2, *trans2))
						{
							ComponentScript* scr1 = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(i);
							ComponentScript* scr2 = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(j);
							//if previous frame is colliding and current frame is colliding for obj1
							//pass in obj2's id
							if (colComp1->collisionEvent[j])
							{
								scr1->OnCollisionStay(j);
							}
							//if previous frame is not colliding and current frame is colliding for obj1
							//pass in obj2's id
							else
							{
								scr1->OnCollisionEnter(j);
								colComp1->collisionEvent[j] = true;
							}
							//if previous frame is colliding and current frame is colliding for obj2
							//pass in obj1's id
							if (colComp2->collisionEvent[i])
							{
								scr2->OnCollisionStay(i);
							}
							//if previous frame is not colliding and current frame is colliding for obj2
							//pass in obj1's id
							else
							{
								scr2->OnCollisionEnter(i);
								colComp2->collisionEvent[i] = true;
							}

							//default response
							DoCollisionResponse(phyComp1, trans1, colComp1, phyComp2, trans2, colComp2);
						}
						//if current frame is not colliding for obj1
						else
						{
							//if previous frame is colliding for obj1
							if (colComp1->collisionEvent[j])
							{
								ComponentScript* scr1 = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(i);

								colComp1->collisionEvent[j] = false;
								scr1->OnCollisionExit(j);
							}
							//if previous frame is colliding for obj2
							if (colComp2->collisionEvent[i])
							{
								ComponentScript* scr2 = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(j);

								colComp2->collisionEvent[i] = false;
								scr2->OnCollisionExit(i);
							}
						}
						
					}
				}
			}
			//physics
			//if (phyComp1->isEnabled())
			//	phyComp1->Update(dt);
			
			// map snap to grid
			//FwMath::AABB aabb =
			//{ (colComp1->vertexA * (trans1->_scale)) + trans1->_currentPosition,
			//	(colComp1->vertexB * (trans1->_scale)) + trans1->_currentPosition };

		}

	}

	void SystemPhysics::Free()
	{
		//std::cout << "SystemPhysics free" << std::endl;
	}

	void SystemPhysics::Destroy()
	{
		//std::cout << "SystemPhysics destroy" << std::endl;
	}
}
