/******************************************************************************/
/*!
\file		SystemPhysics.cpp
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
#include "ISystem.h"
#include "Math_CollisionAABB.h"
#include "Map.h"
#include "Reaction.h"
// EVENT MESSAGING TEST SYSTEM ONLY
#include "SystemObservable.h"
#include "SystemCustomMessages.h"
#include "SystemGraphics.h"
#include "CSVReader.h"
namespace FwEngine
{
	class CSVReader;

	class SystemPhysics : public ISystem
	{

		bool DoCollisionCheck(
			const ComponentPhysics* phys1, const FwMath::AABB aabb1,
			const ComponentPhysics* phys2, const FwMath::AABB aabb2, 
			const float dt, FwMath::Vector3D& normal);
		void DoCollisionResponse(
			ComponentPhysics* phys1, ComponentTransform* trans1, ComponentCollision* coll1,
			ComponentPhysics* phys2, ComponentTransform* trans2, ComponentCollision* coll2);


	public:
		//Gravity of the world
		FwMath::Vector3D _gravity;
		//Max velocity for a physics body
		float _maxVelocity;
		float _maxVelocitySq;
		float _penetrationEpsilon;
		float _penetrationResolvePercentage;
		Map* _map;

		bool _runCollision;
		SystemListener _listener;

		SystemPhysics();
		SystemPhysics(GameObjectPool* gameObjectPool);
		~SystemPhysics();

		void SetMap(Map* map);

		void Init() override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		// EVENT MESSAGING TEST ONLY
		void runCol(InputMessage& msg) 
		{ 
			if (msg._key == IKEY_TAB)
				_runCollision = !_runCollision; 
			if (msg._key == IKEY_SPACE)
			{
				RECT rect;
				GetClientRect(GRAPHICS->GetHandle(), &rect);
				LONG width = rect.right;
				LONG height = rect.bottom;
				if (msg._cursorPos.x < 0 || msg._cursorPos.x > width || msg._cursorPos.y < 0 || msg._cursorPos.y > height)
					return;
				_gameObjectPool->GetContainerPtr<ComponentTransform>()->at(0)._currentPosition = FwMath::Vector3D(
					GRAPHICS->_cameraPos.x + (float)msg._cursorPos.x / (float)width * (float)GRAPHICS->GetWidth(),
					GRAPHICS->_cameraPos.y + GRAPHICS->GetHeight() - ((float)msg._cursorPos.y / (float)height * (float)GRAPHICS->GetHeight())
					, 0.0f);
			}
		}
	};
	extern SystemPhysics* PHYSICS;
}

