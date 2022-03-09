#include "AIBoss1Damaged.h"
#include <memory>

namespace FwEngine
{
	AIBoss1Damaged::AIBoss1Damaged(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ STRING_AI_BOSS_1_DAMAGED, ai, scr, var },
		_compRen{ nullptr },
		_bossIdle{ nullptr }
	{
		if (_compAI)
		{
			_compRen = &_compAI->GetCompScript()->getComponent<ComponentRenderer>();
			_bossIdle = std::dynamic_pointer_cast<AIBoss1Idle>(_compAI->GetAI(STRING_AI_BOSS_1_IDLE));
		}
	}
	AIBoss1Damaged::~AIBoss1Damaged()
	{
	}
	void AIBoss1Damaged::Init()
	{
		if (_bossIdle)
		{
			_bossIdle->_bossPhase++;
		}
	}
	void AIBoss1Damaged::Update(float)
	{
		// Do whatever here, then go back to idle
		if (_bossIdle)
		{
			if (_bossIdle->_bossPhase >= 3)
			{
				// Do stuff here
				_compAI->RunAI("Empty");
				if (_compRen)
					_compRen->_colour = { 0,0,0,1 };
			}
			else
			{
				if (_compRen)
					_compRen->_colour = { 1,1,1,1 };
				_compAI->RunAI(STRING_AI_BOSS_1_IDLE);
			}
		}
	}
	void AIBoss1Damaged::Free()
	{
	}
	void AIBoss1Damaged::OnCollisionEnter(size_t)
	{
	}
	void AIBoss1Damaged::OnCollisionExit(size_t)
	{
	}
	void AIBoss1Damaged::OnCollisionStay(size_t)
	{
	}
}