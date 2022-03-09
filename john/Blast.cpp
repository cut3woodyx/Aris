/******************************************************************************/
/*!
\file		Blast.cpp
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
#include "Blast.h"
#include "SystemInput.h"
#include "ScriptPlayer.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include "SystemManager.h"
#include "Math_CollisionSAT.h"
#include <math.h>

#define ATTRACT IMOUSE_LBUTTON
#define REPEL IMOUSE_RBUTTON
#define DELAY 200
FwEngine::ScriptBlast::ScriptBlast(ComponentScript& comp) : IScript(STRING_SCRIPT_BLAST, comp),
colPhy(_components.getComponent<ComponentPhysics>()),
colAnimation(_components.getComponent<ComponentAnimation>()),
colRender(_components.getComponent<ComponentRenderer>()),
colTrans(_components.getComponent<ComponentTransform>()),
colCollision(_components.getComponent<ComponentCollision>()),
colAudio(_components.getComponent<ComponentAudio>()),
objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
sysMgr{ _components.getSystemManager() },
inp(_components.getSystemInput()),
activated{ false },
initVertexsA{ colCollision._vertexA },
initVertexsB{ colCollision._vertexB },
initVertexsC{ colCollision._vertexC },
initVertexsD{ colCollision._vertexD },
_playerPosition{ nullptr },
playerCollisionComponent{ nullptr },
playerPhysicsComponent{ nullptr },
attractionForce{},
mousePos{ sysMgr._cursorPosition },
vectorDirection{},
isBlue{ sysMgr._playerMgr.isBlue },
horizontalClimb{ false }, 
angle(0),
originB(0.25f),
originR(0.45f),
_magneticStrength{ 1 },
_blastRed{ nullptr },
_blastBlue{ nullptr },
_delay{ DELAY }
{
}

FwEngine::ScriptBlast::~ScriptBlast()
{
}

void FwEngine::ScriptBlast::Init()
{
	_blastRed = objectPool.FindGameObjectByName<ComponentEmitter>("BlastEffect_r");
	_blastBlue = objectPool.FindGameObjectByName<ComponentEmitter>("BlastEffect_b");


	_blastRed->_radius = 0;

	ComponentTransform* player = &objectPool.GetContainerPtr<ComponentTransform>()->at(0);
	size_t playerId= 0;
	const size_t compSize = objectPool.GetContainerPtr<ComponentTransform>()->size();
	for (size_t i = 0; i < compSize; ++player, ++i)
	{
		if (player->_tag == "player")
		{
			playerId = i;
			_playerPosition = player;
			break;
		}
	}
	playerCollisionComponent = &objectPool.GetContainerPtr<ComponentCollision>()->at(playerId);
	playerPhysicsComponent = &objectPool.GetContainerPtr<ComponentPhysics>()->at(playerId);
}

void FwEngine::ScriptBlast::Update(float)
{
	if (_blastRed->GetTransform()->_rotation == 0.0f)
	{
		_blastRed->GetTransform()->_currentPosition.x += _playerPosition->_scale.x * 0.5f ;
	}

	if (_blastRed->GetTransform()->_rotation == 180.0f)
	{
		_blastRed->GetTransform()->_currentPosition.x -= _playerPosition->_scale.x * 0.5f ;
	}



	if ((inp)(REPEL, SystemInput::PRESS) || (inp)(ATTRACT, SystemInput::PRESS))
	{
		_delay.reset();
	}

	if ((inp)(ATTRACT, SystemInput::RELEASE) && horizontalClimb)
	{
		horizontalClimb = false;
		_playerPosition->_currentPosition.y -= 10.0f;
	}
	//setting blast direction
	if ((inp)(IKEY_W, SystemInput::PRESS))
	{
		angle = 90;
		vectorDirection = { 0,1,0 };
	}
	else if ((inp)(IKEY_A, SystemInput::PRESS) && !horizontalClimb)
	{
		angle = 0;
		colRender._flipX = true;
		vectorDirection = { -1,0,0 };
	}
	else if ((inp)(IKEY_D, SystemInput::PRESS) && !horizontalClimb)
	{
		colRender._flipX = false;
		angle = 0;
		vectorDirection = { 1,0,0 };
	}
	if (!playerCollisionComponent->isCollidingWith("heavy metal"))
	{
		horizontalClimb = false;
	}
	//setting collision box
	colCollision._vertexA = initVertexsA;
	colCollision._vertexB = initVertexsB;
	colCollision._vertexC = initVertexsC;
	colCollision._vertexD = initVertexsD;

	//setting up rotation matrix
	FwMath::Matrix3x3 rotation;
	FwMath::Mtx33RotDeg(rotation, angle);

	//rotating
	colCollision._vertexA = rotation * colCollision._vertexA;
	colCollision._vertexB = rotation * colCollision._vertexB;
	colCollision._vertexC = rotation * colCollision._vertexC;
	colCollision._vertexD = rotation * colCollision._vertexD;

	//update rotation
	colTrans._rotation = -angle;

	//update position
	colTrans._currentPosition = _playerPosition->_currentPosition + vectorDirection * 200;

	//if (!((inp)(IMOUSE_LBUTTON, SystemInput::HOLD)))
	//{

	//	//update cursor position if i am not attracting metal
	//	mousePos = sysMgr._cursorPosition;

	//	//setting blast direction
	//	vectorDirection = mousePos - _playerPosition->_currentPosition;
	//	//normalizing
	//	FwMath::Vector3DNormalize(vectorDirection, vectorDirection);

	//	//setting collision box
	//	colCollision._vertexA = initVertexsA;
	//	colCollision._vertexB = initVertexsB;
	//	colCollision._vertexC = initVertexsC;
	//	colCollision._vertexD = initVertexsD;
	//	
	//	float dotProduct = FwMath::Vector3DDotProduct(FwMath::Vector3D{ 1,0,0 }, vectorDirection);
	//	//calculating rotation angle
	//	float determinant = vectorDirection.y;
	//	float angle = atan2f(determinant, dotProduct);

	//	//setting up rotation matrix
	//	FwMath::Matrix3x3 rotation;
	//	FwMath::Mtx33RotRad(rotation, angle);

	//	//rotating
	//	colCollision._vertexA = rotation * colCollision._vertexA;
	//	colCollision._vertexB = rotation * colCollision._vertexB;
	//	colCollision._vertexC = rotation * colCollision._vertexC;
	//	colCollision._vertexD = rotation * colCollision._vertexD;
	//	
	//	//update rotation
	//	colTrans._rotation = -angle / 3.141592654f * 180.0f;
	//}
	//update position
	//colTrans._currentPosition = _playerPosition->_currentPosition + vectorDirection * 200;
}

void FwEngine::ScriptBlast::Free()
{
}

void FwEngine::ScriptBlast::Destroy()
{
}

void FwEngine::ScriptBlast::OnCollisionEnter(size_t)
{
}

void FwEngine::ScriptBlast::OnCollisionExit(size_t)
{
}

void FwEngine::ScriptBlast::OnCollisionStay(size_t id)
{

	if ((inp)(REPEL, SystemInput::HOLD) && _delay.checkTrigger())
	{
		ComponentTransform& transComOther = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		ComponentPhysics& phyComOther = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);

		if (transComOther._tag == "metal")
		{
			//calculatng attraction direction
			FwMath::Vector3D attractedDirection = _playerPosition->_currentPosition - transComOther._currentPosition;

			//calculating attraction force
			FwMath::Vector3D scaleOfCollisionBox = 0.5f * (phyComOther._collisionComponent->_vertexB - phyComOther._collisionComponent->_vertexA);
			float scaleX = scaleOfCollisionBox.x * transComOther._scale.x;
			float scaleY = scaleOfCollisionBox.y * transComOther._scale.y;

			FwMath::Vector3D scaleOfPlayerCollisionBox = 0.5f * (playerCollisionComponent->_vertexB - playerCollisionComponent->_vertexA);
			float scalePlayerX = scaleOfPlayerCollisionBox.x * _playerPosition->_scale.x;
			float scalePlayerY = scaleOfPlayerCollisionBox.y * _playerPosition->_scale.y;

			float offsetA = max(scalePlayerX, scalePlayerY);
			float offsetB = max(scaleX, scaleY);
			float offset = offsetA + offsetB;

			float dist = FwMath::Vector3DLength(attractedDirection);

			float intensity = 1 / ((dist - offset) * (dist - offset));

			//Adjust intensity here if you want stronger or weaker.
			intensity *= 500000000.0f;
			if (intensity >= 1000000)
			{
				intensity = 1000000;
			}

			//FwMath::Vector3DNormalize(attractedDirection, attractedDirection);

			if (_blastBlue)
			{
				float baseValue = originB;
				_blastBlue->_maxLifespan = baseValue * (dist/400);
				
				
			}

			MagneticForce appliedForce{ vectorDirection, intensity * _magneticStrength,0.01f };

			phyComOther.AddForce(appliedForce);

			


		}

		if (transComOther._tag == "heavy metal")
		{

			
			if (!(playerCollisionComponent->isCollidingWith("heavy metal")))
			{
				//magnet pulling player force, the designer can customize
				playerPhysicsComponent->AddForce(MagneticForce{ -vectorDirection, 3000 * _magneticStrength , 0.1f });
			}
		}

	}

	else 
	{
		_blastBlue->_maxLifespan = 0.01f;
	}
	if ((inp)(ATTRACT, SystemInput::HOLD) && _delay.checkTrigger())
	{
		ComponentTransform& transComOther = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		ComponentPhysics& phyComOther = objectPool.GetContainerPtr<ComponentPhysics>()->at(id);

		//calculatng attraction direction
		FwMath::Vector3D attractedDirection = -(_playerPosition->_currentPosition - transComOther._currentPosition);

		if (transComOther._tag == "metal")
		{
			

			//calculating attraction force
			FwMath::Vector3D scaleOfCollisionBox = 0.5f * (phyComOther._collisionComponent->_vertexB - phyComOther._collisionComponent->_vertexA);
			float scaleX = scaleOfCollisionBox.x * transComOther._scale.x;
			float scaleY = scaleOfCollisionBox.y * transComOther._scale.y;

			FwMath::Vector3D scaleOfPlayerCollisionBox = 0.5f * (playerCollisionComponent->_vertexB - playerCollisionComponent->_vertexA);
			float scalePlayerX = scaleOfPlayerCollisionBox.x * _playerPosition->_scale.x;
			float scalePlayerY = scaleOfPlayerCollisionBox.y * _playerPosition->_scale.y;

			float offsetA = max(scalePlayerX, scalePlayerY);
			float offsetB = max(scaleX, scaleY);
			float offset = offsetA + offsetB;

			float dist = FwMath::Vector3DLength(attractedDirection);

			float intensity = 1 / ((dist - offset) * (dist - offset));

			//Adjust intensity here if you want stronger or weaker.
			intensity *= 500000000.0f;
			if (intensity >= 10000)
			{
				intensity = 10000;
			}

			/*FwMath::Vector3DNormalize(attractedDirection, attractedDirection);*/

			if (_blastRed)
			{

				if (dist <= 0) 
				{
					dist = 0;
				}

				if (_blastRed->GetTransform()->_rotation == 0.0f)
				{
					_blastRed->GetTransform()->_currentPosition.x += dist;
				}	

				if (_blastRed->GetTransform()->_rotation == 180.0f) 
				{
					_blastRed->GetTransform()->_currentPosition.x -=  dist;
				}



				float baseValue = originR;
				_blastRed->_maxLifespan = baseValue * (dist / 400);


			}


			MagneticForce appliedForce{ -vectorDirection, intensity * _magneticStrength, 0.10f };

			phyComOther.AddForce(appliedForce);

		}
		if (transComOther._tag == "heavy metal")
		{

			if (vectorDirection.y == 1)
			{
				horizontalClimb = true;
				playerPhysicsComponent->AddForce(MagneticForce{ FwMath::Vector3D(0,1,0), 1000 * _magneticStrength, 0.1f });
			}

			if (!(playerCollisionComponent->isCollidingWith("heavy metal")))
			{
				//magnet pulling player force, the designer can customize
				playerPhysicsComponent->AddForce(MagneticForce{ vectorDirection, 3000 * _magneticStrength , 0.1f });
			}
		}

	}

	else 
	{
		_blastRed->_maxLifespan = 0.02f;
	}
}