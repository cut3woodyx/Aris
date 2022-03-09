/******************************************************************************/
/*!
\file		ScriptPlayerUI.h
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

constexpr auto STRING_SCRIPT_PLAYERUI = "playerui";

namespace FwEngine
{
	class ScriptPlayerUI : public IScript
	{
	protected:
		ComponentPhysics& colPhy;
		ComponentAnimation& colAnimation;
		ComponentRenderer& colRender;
		ComponentTransform& colTrans;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;

		bool& isBlue;
		int& keyAmt;
		int& health;
		int myhealth;
		int myKey;
		bool isChanging;
		bool uiColorRed;
		const float offsetX = 0.5f;
		const float offsetY = 0.3f;
		const float polarityHealthSpacing = 30.0f;
		const float healthSpacing = 80.0f;
		const float healthYOffset = 30.0f;

	public:
		
		ScriptPlayerUI(ComponentScript& comp);

		virtual ~ScriptPlayerUI();

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
