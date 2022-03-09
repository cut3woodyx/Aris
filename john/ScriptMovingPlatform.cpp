/******************************************************************************/
/*!
\file		ScirptMovingPlatform.cpp
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "ScriptPlayer.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "ScriptMovingPlatform.h"
#include "SystemManager.h"

namespace FwEngine
{
	ScriptMovingPlatform::ScriptMovingPlatform(ComponentScript& comp) : IScript(STRING_SCRIPT_MOVINGPLATFORM, comp),
		colText(_components.getComponent<ComponentText>()),
		colTrans(_components.getComponent<ComponentTransform>()),
		colCol(_components.getComponent<ComponentCollision>()),
		//colPhy(_components.getComponent<ComponentPhysics>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager()),
		sysScr(_components.getSystemScript()),
		audio(_components.getComponent<ComponentAudio>()),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),

		_scrply(nullptr),

		_startPos(0.0f, 0.0f, 0.0f),

		_waypointPos(0.0f, 0.0f, 0.0f),
		_wayPointX(0.0f),
		_wayPointY(0.0f),

		_velocityMax(0.0f, 0.0f, 0.0f),
		_accleration(0.0f, 0.0f, 0.0f),

		_wayPointState(0),

		_movement(
			FwMath::Vector3D(0.0f, 0.0f, 0.0f), //initialPos
			FwMath::Vector3D(0.0f, 0.0f, 0.0f), //destination
			FwMath::Vector3D(0.0f, 0.0f, 0.0f), //velocityMax
			FwMath::Vector3D(0.0f, 0.0f, 0.0f)  //accleration
		),
		_lastPos(0.0f, 0.0f, 0.0f)
	{
		//AddStringVar("waypoint", &_stringWayPoint);
		AddFloatVar("wayPointX", &_wayPointX);
		AddFloatVar("wayPointY", &_wayPointY);
	}

	ScriptMovingPlatform::~ScriptMovingPlatform()
	{
	}

	void ScriptMovingPlatform::Init()
	{
		GameObject* playerObj = objectPool.FindGameObjectByTag<GameObject>("player");
		_scrply = reinterpret_cast<ScriptPlayer*>(sysScr.FindScript((int)playerObj->GetIndex(), "player"));


		_startPos = colTrans._currentPosition;

		_waypointPos = FwMath::Vector3D(_wayPointX, _wayPointY, 0.0f);

		_velocityMax = FwMath::Vector3D(300.0f, 300.0f, 0.0f);
		_accleration = FwMath::Vector3D(100.0f, 100.0f, 0.0f);


		_movement.setVelocityMax(_velocityMax);
		_movement.setAccleration(_accleration);

		_movement.setCurrentPos(_startPos);
		_movement.reset(_startPos, _waypointPos);

		_wayPointState = 0;

		_lastPos = _startPos;

		//const char* delimChar = ",";
		//char* str = _stringWayPoint.
		//char* token = strtok(str, delimChar);

		//while (token != nullptr)
		//{

		//	token = strtok(nullptr, delimChar);
		//}




	}

	void ScriptMovingPlatform::Update(float dt) // dt
	{
		//colTrans._currentPosition.x += 1.0f;
		//colTrans._currentPosition.y += 1.0f;

		_lastPos = colTrans._currentPosition;
		colTrans._currentPosition = _movement.step(dt);

		//_movement.step(dt);
		//colTrans._currentPosition += _movement.getLastDiff();

		if (_wayPointState == 0)
		{
			if (_movement.hasReached())
			{
				_movement.reset(_waypointPos, _startPos); // move back
				_wayPointState = 1;
			}
		}
		else if (_wayPointState == 1)
		{
			if (_movement.hasReached())
			{
				_movement.reset(_startPos, _waypointPos); // move to
				_wayPointState = 0;
			}
		}

	}

	void ScriptMovingPlatform::Free()
	{
	}

	void ScriptMovingPlatform::Destroy()
	{
	}

	void ScriptMovingPlatform::Unload()
	{
		colTrans._currentPosition = _startPos; // 
	}

	void ScriptMovingPlatform::OnCollisionEnter(size_t)
	{

		//colTrans._currentPosition.x -= 1.0f;
	}

	void ScriptMovingPlatform::OnCollisionExit(size_t )
	{
		////col.Collision_Check_List();
		//ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		//if (
		//	otherTransform._tag == "non metal" ||
		//	otherTransform._tag == "heavy metal" ||
		//	otherTransform._tag == "metal" ||
		//	otherTransform._tag == "player"
		//	)
		//{

		//	ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);

		//	// nudge diff
		//	FwMath::Vector3D diff = colTrans._currentPosition - _lastPos;

		//	//DirectionalForce PushForce(diff, diff.length(), 0.85f);

		//	//otherPhysics._velocity += diff * 1000.0f;
		//	otherPhysics.AddForce(diff * 100.0f);
		//}

	}

	void ScriptMovingPlatform::OnCollisionStay(size_t id)
	{
		//col.Collision_Check_List();
		ComponentTransform& otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		if (
			otherTransform._tag == "non metal" ||
			otherTransform._tag == "heavy metal" ||
			otherTransform._tag == "metal" ||
			otherTransform._tag == "player" 
			)
		{
			// nudge diff
			FwMath::Vector3D diff = colTrans._currentPosition - _lastPos;

			ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);

			float otherBot = otherTransform._currentPosition.y - abs(otherTransform._scale.y * otherCollision._vertexA.y);
			//otherBot += diff.y;

			float otherTop = otherTransform._currentPosition.y + abs(otherTransform._scale.y * otherCollision._vertexB.y);
			//otherTop += diff.y;

			float thisTop = colTrans._currentPosition.y + abs(colTrans._scale.y * colCol._vertexB.y);

			float thisBot = colTrans._currentPosition.y - abs(colTrans._scale.y * colCol._vertexA.y);

			float diffyTop = abs(otherBot - thisTop);
			float diffyBot = abs(thisBot - otherTop);

			//if (diffyBot < diffyTop)
			//	return; 

			//float diffy = diffyTop;

			float otherLeft = otherTransform._currentPosition.x - abs(otherTransform._scale.x * otherCollision._vertexA.x);
			//otherLeft += diff.x;

			float otherRight = otherTransform._currentPosition.x + abs(otherTransform._scale.x * otherCollision._vertexB.x);
			//otherRight += diff.x;

			float thisLeft = colTrans._currentPosition.x - abs(colTrans._scale.x * colCol._vertexA.x);
			float thisRight = colTrans._currentPosition.x + abs(colTrans._scale.x * colCol._vertexB.x);

			float diffx1 = otherLeft - thisRight;
			float diffx2 = thisLeft - otherRight;


			//ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
			// no physics involved.. for now
			
			// not sure if this is bugggged !!
			if (diffyBot < diffyTop)
			{
				// DO check for - player squashed by platform

				// check other obj x pos within x bounds of platform

				if (otherTransform._currentPosition.x >= thisLeft && otherTransform._currentPosition.x <= thisRight)
				{
					// squashed
					//player 
					if (otherTransform._tag == "player")
					{
						//_scrply->_health--;
						_scrply->PlayerGetDamaged(1);
						
						otherTransform._currentPosition.y = //thisTop 
							colTrans._currentPosition.y + abs(colTrans._scale.y * colCol._vertexB.y)
							+ abs(otherTransform._scale.y * otherCollision._vertexA.y) /*- 0.01f*/;
					}


					return;
				}



				// check push (snap to) left or right of platform
				if (abs(diffx1) > abs(diffx2))
				{
					otherTransform._currentPosition.x = thisLeft - abs(otherTransform._scale.x * otherCollision._vertexB.x);
				}
				else
				{
					otherTransform._currentPosition.x = thisRight + abs(otherTransform._scale.x * otherCollision._vertexA.x);
				}
				return;
			}

			// // do "stacking" response

			ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
			otherPhysics._velocity.y = 0.0f; // set y velocity to 0
			//otherPhysics._velocity = FwMath::Vector3D(0.0f, 0.0f, 0.0f);

			otherTransform._currentPosition.y = //thisTop 
				colTrans._currentPosition.y + abs(colTrans._scale.y * colCol._vertexB.y) 
				+ abs(otherTransform._scale.y * otherCollision._vertexA.y) /*- 0.01f*/;

			// player 
			//if (otherTransform._tag == "player")
			//{
			//}

			// // nudge

			//otherTransform._currentPosition.x += 1.0f;
			//otherTransform._currentPosition.y += 1.0f;
			//FwMath::Vector3D v = _movement.getLastDiff();
			//otherTransform._currentPosition.x += v.x;
			//otherTransform._currentPosition.y += v.y;
			//otherTransform._currentPosition += _movement.getLastDiff();


			// // nudge
			otherTransform._currentPosition += diff;



			// default stacking
			// needs physics
			//ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
			//ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
			//StackingResponse_NonElastic(&colPhy, &colTrans, &colCol, &otherPhysics, &otherTransform, &otherCollision);

		}


		// mplatf has no physics
		//if (otherTransform._tag == "Door")
		//{
		//	ComponentCollision& otherCollision = objectPool.GetContainerPtr<ComponentCollision>()->at(id);
		//	ComponentPhysics& otherPhysics = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);
		//	StackingResponse_NonElastic(&colPhy, &colTrans, &colCol, &otherPhysics, &otherTransform, &otherCollision);
		//}
	}

	void ScriptMovingPlatform::OnEnterScn()
	{

	}

	void ScriptMovingPlatform::PushVars()
	{
		if (_originalVars.size() > 0) {
			_wayPointX = stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
		if (_originalVars.size() > 0) {
			_wayPointY = stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
	}





	// movement


	Movement::Movement(
		float initialPos, 
		float destination, 
		//float velocityMin, // magnitude
		float velocityMax, // magnitude
		float accleration  // magnitude
	) :
		_currentPos(initialPos),
		_currentVelocity(0.0f),
		_initialPos(initialPos),
		_destination(destination),
		//_velocityMin(abs(velocityMin)),
		_velocityMax(abs(velocityMax)),
		_accleration(abs(accleration)),
		_hasReached(false),
		_vector(1.0f),

		_lastdiff(0.0f)
	{
		if ((_destination - _initialPos) < 0.0f)
		{
			_vector = -1.0f;
		}
	}

	float Movement::step(float dt)
	{
		if (_hasReached)
			return _currentPos;


		_currentVelocity += _accleration * dt * _vector;
		if (abs(_currentVelocity) > _velocityMax)
			_currentVelocity = _velocityMax * _vector;

		float nextPos = _currentPos + _currentVelocity * dt;
		float distToNextPos = abs(nextPos - _currentPos);
		float distToDest = abs(_destination - _currentPos);

		if(distToNextPos >= distToDest)
		{
			_currentVelocity = 0.0f;

			_hasReached = true;

			_lastdiff = _destination - _currentPos;

			//_currentPos = _destination;
			_currentPos += _lastdiff;

			return _currentPos;
		}
		
		//_currentPos += _currentVelocity * dt;

		_lastdiff = nextPos - _currentPos;

		//_currentPos = nextPos;
		_currentPos += _lastdiff;

		return _currentPos;
	}

	bool Movement::hasReached()
	{
		return _hasReached;
	}

	void Movement::reset(float initialPos, float destination)
	{
		_hasReached = false;
		_initialPos = initialPos;
		_destination = destination;

		_currentVelocity = 0.0f;

		if ((_destination - _initialPos) < 0.0f)
		{
			_vector = -1.0f;
		}
		else
		{
			_vector = 1.0f;
		}
	}

	void Movement::setCurrentPos(float pos)
	{
		_currentPos = pos;
	}

	void Movement::setVelocityMax(float vel)
	{
		_velocityMax = vel;
	}

	void Movement::setAccleration(float acc)
	{
		_accleration = acc;
	}

	float Movement::getLastDiff() // dt?
	{
		return _lastdiff;
	}

	Movement2D::Movement2D(
		FwMath::Vector3D initialPos, 
		FwMath::Vector3D destination, 
		FwMath::Vector3D velocityMax, 
		FwMath::Vector3D accleration
	) :
		_movementX(initialPos.x, destination.x, velocityMax.x, accleration.x),
		_movementY(initialPos.y, destination.y, velocityMax.y, accleration.y)
	{
	}

	FwMath::Vector3D Movement2D::step(float dt)
	{
		float x = _movementX.step(dt);
		float y = _movementY.step(dt);
		return FwMath::Vector3D(x, y, 0.0f);
	}

	bool Movement2D::hasReached()
	{
		return _movementX.hasReached() && _movementY.hasReached();
	}

	void Movement2D::reset(FwMath::Vector3D initialPos, FwMath::Vector3D destination)
	{
		_movementX.reset(initialPos.x, destination.x);
		_movementY.reset(initialPos.y, destination.y);
	}

	void Movement2D::setCurrentPos(FwMath::Vector3D pos)
	{
		_movementX.setCurrentPos(pos.x);
		_movementY.setCurrentPos(pos.y);
	}

	void Movement2D::setVelocityMax(FwMath::Vector3D vel)
	{
		_movementX.setVelocityMax(vel.x);
		_movementY.setVelocityMax(vel.y);
	}

	void Movement2D::setAccleration(FwMath::Vector3D acc)
	{
		_movementX.setAccleration(acc.x);
		_movementY.setAccleration(acc.y);
	}

	FwMath::Vector3D Movement2D::getLastDiff()
	{
		return FwMath::Vector3D(
			_movementX.getLastDiff(),
			_movementY.getLastDiff(),
			0.0f
		);
	}

}
