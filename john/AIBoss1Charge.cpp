#include "AIBoss1Charge.h"
#include "AIBoss1Attack.h"
#include "AIBoss1Vulnerable.h"

namespace FwEngine
{
	AIBoss1Charge::AIBoss1Charge(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ STRING_AI_BOSS_1_CHARGE, ai, scr, var },
		_compTrans{ nullptr },
		_input{ nullptr },
		_currentTime{ 0 },
		_metalEaten{ 0 }
	{
		if (_compAI)
		{
			_compTrans = &_compAI->GetCompScript()->getComponent<ComponentTransform>();
			_input = &_compAI->GetCompScript()->getSystemInput();
		}
	}
	AIBoss1Charge::~AIBoss1Charge()
	{
	}
	void AIBoss1Charge::Init()
	{
		_metalEaten = 0;
		_currentTime = 0;
		// Run animation here?
	}
	void AIBoss1Charge::Update(float dt)
	{
		_currentTime += dt;
		if (_currentTime < 3)
		{
			// Move here?
			_compTrans->_currentPosition.x -= 100.0f * dt;
			if (_input->GetKeyIf(IKEY_SPACE, SystemInput::PRESS))
				_metalEaten = 3;
		}
		else
		{
			_compAI->RunAI(STRING_AI_BOSS_1_ATTACK);
		}

		if (_metalEaten >= 3)
		{
			_compAI->RunAI(STRING_AI_BOSS_1_VULN);
		}
	}
	void AIBoss1Charge::Free()
	{
	}
	void AIBoss1Charge::OnCollisionEnter(size_t)
	{
		// Box eating here?
	}
	void AIBoss1Charge::OnCollisionExit(size_t)
	{
	}
	void AIBoss1Charge::OnCollisionStay(size_t)
	{
	}
}