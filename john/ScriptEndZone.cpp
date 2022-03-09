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
#include "ScriptEndZone.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "ScriptDebug.h"
#include "SystemManager.h"

namespace FwEngine
{
	ScriptEndZone::ScriptEndZone(ComponentScript& comp) : IScript(STRING_SCRIPT_ENDZONE, comp),
		colRender(_components.getComponent<ComponentRenderer>()),
		calTrans(_components.getComponent<ComponentTransform>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager()),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool())),
		_endZone{ nullptr },
		_triggered(false),
		_timer(2000),
		_pColT(nullptr)
	{
	}

	ScriptEndZone::~ScriptEndZone()
	{
	}

	void ScriptEndZone::Init()
	{
		_endZone = objectPool.FindGameObjectByTag<ComponentRenderer>("EndZone");
		if (_endZone)
			_endZone->disable();
	}

	void ScriptEndZone::Update(float)
	{
		if (!_triggered)
			return;

		if (_endZone == nullptr)
		{
			if (_timer.checkTrigger())
				//sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_RETURNTOMENU;
			{
				if (_pColT)
				{
					if (_pColT->_tag == "player")
					{
						//audio.Play("Win");
						//sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERWON;
						//sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;

						sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
						sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_NEXTSCENE;

						// reset event keys
						sysMgr._playerMgr.keys = 0;
						sysMgr._playerMgr.keyLocation.clear();
						sysMgr._playerMgr.isRestrickMovement = false;
						sysMgr._playerMgr.isShakeEvent = false;
						sysMgr._playerMgr.health = 5;

						sysMgr._objectPoolNames.clear();

						std::string tag = calTrans._tag;

						if (tag.substr(0, 7).find("Portal_") != std::string::npos)
						{
							tag = tag.substr(7, tag.size());
							sysMgr._nextScene = tag;
						}

					}
				}
			}
		}
		else if (_endZone->isEnabled() && !_endZone->GetAnimation()->IsPlaying())
		{
			if(_timer.checkTrigger())
				//sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_RETURNTOMENU;
			{
				if (_pColT)
				{
					if (_pColT->_tag == "player")
					{
						//audio.Play("Win");
						//sysMgr._menustateNext = sysMgr.MENUSTATE_PLAYERWON;
						//sysMgr._gamestateNext = sysMgr.GAMESTATE_PAUSE;

						sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
						sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_NEXTSCENE;

						// reset event keys
						sysMgr._playerMgr.keys = 0;
						sysMgr._playerMgr.keyLocation.clear();
						sysMgr._playerMgr.isRestrickMovement = false;
						sysMgr._playerMgr.isShakeEvent = false;
						sysMgr._playerMgr.health = 5;

						sysMgr._objectPoolNames.clear();

						std::string tag = calTrans._tag;

						if (tag.substr(0, 7).find("Portal_") != std::string::npos)
						{
							tag = tag.substr(7, tag.size());
							sysMgr._nextScene = tag;
						}

					}
				}
			}
		}

	}

	void ScriptEndZone::Unload()
	{
		if (_endZone)
		{
			_endZone->enable();
			_endZone->GetAnimation()->_currentAnimationName = "";
		}
	}

	void ScriptEndZone::Free()
	{
	}

	void ScriptEndZone::Destroy()
	{
	}

	void ScriptEndZone::OnCollisionEnter(size_t id)
	{

		ComponentTransform* pTrans = objectPool.GetComponent<ComponentTransform>(id);
		if (pTrans->_tag._Equal("player"))
		{
			_pColT = pTrans;
			if (_endZone)
			{
				_endZone->enable();
				_endZone->GetAnimation()->PlayAnimation("End", false);
				_timer.reset();
			}
			_triggered = true;
		}


	}

	void ScriptEndZone::OnCollisionExit(size_t)
	{
	}

	void ScriptEndZone::OnCollisionStay(size_t)
	{
	}
}

