/******************************************************************************/
/*!
\file		ScriptMenuUIButton.h
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

constexpr auto STRING_SCRIPT_MENUUIBUTTON = "menuuibutton";

namespace FwEngine
{
	class ScriptMenuUIButton : public IScript
	{
	protected:
		ComponentRenderer& colRender;
		ComponentAnimation& colAnimation;
		ComponentTransform& colTrans;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;

		FwMath::Vector3D _verticeA; 
		FwMath::Vector3D _verticeB;

		bool isWithinBounds(FwMath::Vector3D cursor, FwMath::Vector3D verticeA, FwMath::Vector3D verticeB);

		void options(FwMath::Vector3D menuCursorPos);

		void credits(FwMath::Vector3D menuCursorPos);

	public:
		
		ScriptMenuUIButton(ComponentScript& comp);

		virtual ~ScriptMenuUIButton();

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
