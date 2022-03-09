/******************************************************************************/
/*!
\file		ScirptWinZone.cpp
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
#include "ScriptPlayer.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "ScriptWinZone.h"
#include "SystemManager.h"

constexpr int INACTIVEPORTALTIMER = /*2500*/ 1250;

namespace FwEngine
{
	ScriptWinZone::ScriptWinZone(ComponentScript& comp) : IScript(STRING_SCRIPT_WINZONE, comp),
		colText(_components.getComponent<ComponentText>()),
		colTrans(_components.getComponent<ComponentTransform>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager()),
		audio(_components.getComponent<ComponentAudio>()),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
		_onEnterSceneTimer(INACTIVEPORTALTIMER),
		_x(0.0f),
		_y(0.0f)
	{
		AddFloatVar("X", &_x);
		AddFloatVar("Y", &_y);
	}

	ScriptWinZone::~ScriptWinZone()
	{
	}

	void ScriptWinZone::Init()
	{
	}

	void ScriptWinZone::Update(float)
	{
	}

	void ScriptWinZone::Free()
	{
	}

	void ScriptWinZone::Destroy()
	{
	}

	void ScriptWinZone::OnCollisionEnter(size_t id)
	{

		if (!_onEnterSceneTimer.checkTrigger())
			return;

		ComponentTransform* colt = objectPool.GetComponent<ComponentTransform>(id);
		if (colt->_tag == "player")
		{

			std::string tag = colTrans._tag;

			if (tag.substr(0, 7).find("Portal_") != std::string::npos)
			{
				tag = tag.substr(7, tag.size());

				if (sysMgr._playerMgr.keys >= 3 && sysMgr._playerMgr.isRestrickMovement)
				{
					if (
						tag._Equal("GeneratorRoom") ||
						tag._Equal("TrainingRoom") ||
						tag._Equal("ResearchRoom")
						)
					{
						return;
					}
				}

				sysMgr._nextScene = tag;

				if (sysMgr._nextScene._Equal("Elevator") && sysMgr._playerMgr.keys >= 3)
				{
					sysMgr._playerMgr.isRestrickMovement = true;
				}

				//audio.Play("Win");
				sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERWON;
				sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;
			}


		}
	}

	void ScriptWinZone::OnCollisionExit(size_t)
	{
	}

	void ScriptWinZone::OnCollisionStay(size_t)
	{
	}

	void ScriptWinZone::OnEnterScn()
	{
		_onEnterSceneTimer.reset();
	}

	void ScriptWinZone::PushVars()
	{
		if (_originalVars.size() > 0) {
			_x = std::stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
		if (_originalVars.size() > 0) {
			_y = std::stof(_originalVars[0]);
			_originalVars.erase(std::begin(_originalVars));
		}
	}

}
