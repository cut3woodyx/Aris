/******************************************************************************/
/*!
\file		ScriptMovingPlatform.h
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
#pragma once

#include "IScript.h"
#include "FWMath.h"

constexpr auto STRING_SCRIPT_MOVINGPLATFORM = "movingplatform";

namespace FwEngine
{

	class Movement
	{
		float _currentPos;
		float _currentVelocity;
		
		float _initialPos;
		float _destination;
		//float _velocityMin;
		float _velocityMax;
		float _accleration;

		bool _hasReached;

		float _vector;

		float _lastdiff;

	public:
		Movement(
			float initialPos,
			float destination,
			//float velocityMin,
			float velocityMax,
			float accleration
		);

		float step(float dt);

		bool hasReached();

		void reset(
			float initialPos,
			float destination
		);

		void setCurrentPos(float pos);

		void setVelocityMax(float vel);

		void setAccleration(float acc);

		float getLastDiff();

	};

	class Movement2D
	{
		Movement _movementX;
		Movement _movementY;

	public:
		Movement2D(
			FwMath::Vector3D initialPos,
			FwMath::Vector3D destination,
			FwMath::Vector3D velocityMax,
			FwMath::Vector3D accleration
		);

		FwMath::Vector3D step(float dt);

		bool hasReached();

		void reset(
			FwMath::Vector3D initialPos,
			FwMath::Vector3D destination
		);

		void setCurrentPos(FwMath::Vector3D pos);

		void setVelocityMax(FwMath::Vector3D vel);

		void setAccleration(FwMath::Vector3D acc);

		FwMath::Vector3D getLastDiff();

	};

	class ScriptMovingPlatform : public IScript
	{
	protected:
		typedef Movement2D MPMovement;

		ComponentTransform& colTrans;
		ComponentText& colText;
		ComponentCollision& colCol;
		//ComponentPhysics& colPhy;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;
		SystemScript& sysScr;
		ComponentAudio& audio;
		GameObjectPool& objectPool;

		ScriptPlayer* _scrply;

		//std::string _stringWayPoint;

		FwMath::Vector3D _startPos;

		FwMath::Vector3D _waypointPos;
		float _wayPointX;
		float _wayPointY;

		FwMath::Vector3D _velocityMax;
		FwMath::Vector3D _accleration;

		int _wayPointState;


		MPMovement _movement;

		FwMath::Vector3D _lastPos;



	public:



		ScriptMovingPlatform(ComponentScript& comp);

		virtual ~ScriptMovingPlatform();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void Unload() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void PushVars();

		virtual void OnEnterScn() override;

	};

}
