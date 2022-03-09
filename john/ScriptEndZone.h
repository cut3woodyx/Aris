/******************************************************************************/
/*!
\file		ScriptEndZone.cpp
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

constexpr auto STRING_SCRIPT_ENDZONE = "endzone";

namespace FwEngine
{
	class ScriptEndZone : public IScript
	{
	protected:
		ComponentRenderer& colRender;
		ComponentTransform& calTrans;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;
		GameObjectPool& objectPool;
		
		ComponentRenderer* _endZone;

		bool _triggered;
		Timer _timer;

		ComponentTransform* _pColT;

	public:

		ScriptEndZone(ComponentScript& comp);

		virtual ~ScriptEndZone();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Unload() override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

	};

}
