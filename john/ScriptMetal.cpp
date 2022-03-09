/******************************************************************************/
/*!
\file		ScriptMetal.cpp
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
#include "ScriptMetal.h"
#include "Reaction.h"
namespace FwEngine
{
	ScriptMetal::ScriptMetal(ComponentScript& comp):IScript(STRING_SCRIPT_METAL, comp),
		colRen { _components.getComponent<ComponentRenderer>() },
		colAnim{ _components.getComponent<ComponentAnimation>() },
		colPhy{ _components.getComponent<ComponentPhysics>() },
		_prevAnim{},
		_idleName{},
		_startName{},
		_loopName{}
		/*
		colCollision(_components.getComponent<ComponentCollision>()),
		colTransform(_components.getComponent<ComponentTransform>()),
		colPhysics(_components.getComponent<ComponentPhysics>()),
		objectPool(*(_components.getParentGameObject()->GetParentObjectPool()))
		*/
	{
		AddStringVar("Idle Name", &_idleName);
		AddStringVar("Start Name", &_startName);
		AddStringVar("Loop name", &_loopName);
		//AddStringVar("List of AI", &_animName);
		//AddStringVar("List of Variables (1)", &_listOfVars);
	}

	ScriptMetal::~ScriptMetal()
	{
	}
	void ScriptMetal::Init()
	{
	}
	void ScriptMetal::Update(float)
	{
		int startNum = 0, loopNum = 0;

		for (unsigned i = 0; i < colAnim._animationNames.size(); ++i)
		{
			if (_startName == colAnim._animationNames[i])
				startNum = colAnim._animationsFrames[i];
			if (_loopName == colAnim._animationNames[i])
				loopNum = colAnim._animationsFrames[i];
		}

		if (startNum < 1)
			startNum = 1;
		if (loopNum < 1)
			loopNum = 1;

		auto playAnims = [&]()
		{
			if (_prevAnim.empty())
			{
				colAnim.PlayAnimation(_startName, false);
				_prevAnim = _startName;
			}
			else if (!colAnim._play)
			{
				if (_prevAnim == _startName)
				{
					if (!colAnim._reverse)
						colAnim.PlayAnimation(_loopName, false);
					else
					{
						colAnim.StopAnimation();
						colAnim.PlayAnimation(_startName, false);
					}
				}
				else if (_prevAnim == _loopName)
				{
					if (!colAnim._reverse)
					{
						colAnim.StopAnimation();
						colAnim.PlayAnimation(_loopName, false, 1, loopNum-1,true);
					}
					else
					{
						colAnim.StopAnimation();
						colAnim.PlayAnimation(_loopName, false);
					}
				}
				_prevAnim = colAnim._currentAnimationName;
			}
		};
		// Going left
		if (colPhy._velocity.x < -1.0f)
		{
			if (!colRen._flipX)
			{
				colRen._flipX = true;
				_prevAnim.clear();
			}
			playAnims();
			//_prevAnim = colAnim._currentAnimationName;
		}
		// Going right
		else if (colPhy._velocity.x > 1.0f)
		{
			if (colRen._flipX)
			{
				colRen._flipX = false;
				_prevAnim.clear();
			}
			playAnims();
			//_prevAnim = colAnim._currentAnimationName;
		}
		// Not moving
		else
		{
			//colAnim.PlayAnimation("");
			//colAnim.PlayAnimation(_animName, false, true);

			if (!colAnim._play)
			{
				if (_prevAnim == _loopName)
				{
					if (colAnim._reverse)
					{
						colAnim.PlayAnimation(_startName, false, 0, startNum-1, true);
						_prevAnim = _startName;
					}
					else
					{
						colAnim.StopAnimation();
						colAnim.PlayAnimation(_loopName, false, 1, loopNum-1, true);
					}
				}
				else if (_prevAnim == _startName)
				{
					if (colAnim._reverse)
					{
						colAnim.PlayAnimation(_idleName, true);
						_prevAnim.clear();
					}
					else
					{
						colAnim.StopAnimation();
						colAnim.PlayAnimation(_startName, false, 0, startNum-1, true);
						_prevAnim = _startName;
					}
				}
			}
		}
	}
	void ScriptMetal::Free()
	{
	}
	void ScriptMetal::Destroy()
	{
	}
	void ScriptMetal::OnCollisionEnter(size_t)
	{
		/*
		ComponentTransform otherTransform = objectPool.GetContainerPtr<ComponentTransform>()->at(id);
		if (otherTransform._tag=="player")
		{
			colTransform._currentPosition = colPhysics._previousPosition;
		}
		*/
	}
	void ScriptMetal::OnCollisionExit(size_t)
	{

	}
	void ScriptMetal::OnCollisionStay(size_t)
	{
	}
	void ScriptMetal::PushVars()
	{
		if (_originalVars.size())
		{
			_idleName = _originalVars[0];
			_originalVars.erase(_originalVars.begin());
		}
		if (_originalVars.size())
		{
			_startName = _originalVars[0];
			_originalVars.erase(_originalVars.begin());
		}
		if (_originalVars.size())
		{
			_loopName = _originalVars[0];
			_originalVars.erase(_originalVars.begin());
		}
	}
}