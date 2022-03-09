/******************************************************************************/
/*!
\file		ScriptMetal.h
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

constexpr auto STRING_SCRIPT_METAL = "metal";

namespace FwEngine
{
	class ScriptMetal : public IScript
	{
	protected:
		ComponentRenderer& colRen;
		ComponentAnimation& colAnim;
		ComponentPhysics& colPhy;

		std::string _prevAnim;

		std::string _idleName;
		std::string _startName;
		std::string _loopName;

		/*
		ComponentCollision& colCollision;
		ComponentTransform& colTransform;
		GameObjectPool& objectPool;
		*/
		
	public:

		ScriptMetal(ComponentScript& comp);

		virtual ~ScriptMetal();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void PushVars() override;
	};

}
