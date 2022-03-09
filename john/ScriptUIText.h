/******************************************************************************/
/*!
\file		ScirptUIText.h
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

constexpr auto STRING_SCRIPT_UITEXT = "uitext";

namespace FwEngine
{
	class ScriptUIText : public IScript
	{
	protected:
		ComponentTransform& colTrans;
		ComponentText& colText;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;

	public:
		
		ScriptUIText(ComponentScript& comp);

		virtual ~ScriptUIText();

		virtual void Init() override;
		
		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

	};

}
