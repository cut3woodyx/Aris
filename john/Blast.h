/******************************************************************************/
/*!
\file		Blast.h
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
#include "IScript.h"

constexpr auto STRING_SCRIPT_BLAST = "blast";

namespace FwEngine
{
	class ScriptBlast : public IScript
	{
	protected:
		ComponentPhysics& colPhy;
		ComponentAnimation& colAnimation;
		ComponentRenderer& colRender;
		ComponentTransform& colTrans;
		ComponentCollision& colCollision;
		ComponentAudio& colAudio;
		GameObjectPool& objectPool;
		SystemManager& sysMgr;
		SystemInput& inp;
		bool activated;
		Vector3D initVertexsA;
		Vector3D initVertexsB;
		Vector3D initVertexsC;
		Vector3D initVertexsD;
		ComponentTransform* _playerPosition;
		ComponentCollision* playerCollisionComponent;
		ComponentPhysics* playerPhysicsComponent;


		ComponentEmitter* _blastRed;
		ComponentEmitter* _blastBlue;

		DirectionalForce attractionForce;
		FwMath::Vector3D mousePos;
		FwMath::Vector3D vectorDirection;
		Timer _delay;
		bool& isBlue;
		bool horizontalClimb;
		float angle;
		float originR;
		float originB;
		

	public:
		float _magneticStrength;

		ScriptBlast(ComponentScript& comp);

		virtual ~ScriptBlast();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
	};

}
