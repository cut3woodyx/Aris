/******************************************************************************/
/*!
\file		ScriptTestObjectOne.h
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

#include "SystemInput.h"
#include "SystemGraphics.h"

constexpr auto STRING_SCRIPT_TESTOBJECTONE = "testobjectone";

namespace FwEngine
{
	class ScriptTestObjectOne : public IScript
	{
	protected:
		ComponentPhysics& _colPhy;
		ComponentAnimation& _colAnimation;
		ComponentRenderer& _colRender;
		ComponentTransform& _colTransform;

		SystemInput& _inp;
		SystemGraphics& _gfx;
	public:
		ScriptTestObjectOne(ComponentScript& comp);
		virtual ~ScriptTestObjectOne();

		virtual void Init();
		virtual void Update(float dt);
		virtual void Free();
		virtual void Destroy();

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		//virtual void OnTriggerEnter(int id) override;
		//virtual void OnTriggerExit(int id) override;
		//virtual void OnTriggerStay(int id) override;
	};

}
