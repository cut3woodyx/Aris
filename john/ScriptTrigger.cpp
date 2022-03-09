/******************************************************************************/
/*!
\file		ScriptTrigger.cpp
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
#include "ScriptTrigger.h"
#include "ScriptPlayer.h"
#include <string>

namespace FwEngine
{
	ScriptTrigger::ScriptTrigger(ComponentScript& comp) :
		IScript{ STRING_SCRIPT_TRIGGER, comp },
		_sysGraphics{ comp.getSystemGraphics() },
		_objectPool{ (*(_components.getParentGameObject()->GetParentObjectPool())) },
		_timer{ 0 }
	{
	}

	ScriptTrigger::~ScriptTrigger()
	{
	}

	void ScriptTrigger::Init()
	{
	}

	void ScriptTrigger::Update(float dt)
	{
		if (_timer > 0.0f)
		{
			_timer -= dt;
		}
			
	}

	void ScriptTrigger::Free()
	{
	}

	void ScriptTrigger::Destroy()
	{
	}

	void ScriptTrigger::OnCollisionEnter(size_t id)
	{
		std::string tag{ _objectPool.GetContainerPtr<ComponentTransform>()->at(id)._tag };
		if (tag._Equal(STRING_SCRIPT_PLAYER))
		{
			_timer = CAMERA_SHAKE_TIMER;
		}
		if (tag._Equal("lift"))
		{
			tag = "Done";
		}
	}

	void ScriptTrigger::OnCollisionExit(size_t)
	{
	}

	void ScriptTrigger::OnCollisionStay(size_t)
	{
	}
}
