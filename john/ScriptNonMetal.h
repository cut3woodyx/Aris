/******************************************************************************/
/*!
\file		ScriptNonMetal.h
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

constexpr auto STRING_SCRIPT_NON_METAL = "non metal";

namespace FwEngine
{
	class ScriptNonMetal : public IScript
	{
	protected:
		ComponentAnimation& colAnim;
		ComponentPhysics& colPhy;
		/*
		ComponentCollision& colCollision;
		ComponentTransform& colTransform;
		ComponentPhysics& colPhysics;
		GameObjectPool& objectPool;
		*/

	public:

		ScriptNonMetal(ComponentScript& comp);

		virtual ~ScriptNonMetal();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
	};

}
