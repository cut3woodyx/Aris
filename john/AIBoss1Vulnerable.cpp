#include "AIBoss1Vulnerable.h"
#include "AIBoss1Damaged.h"

namespace FwEngine
{
	AIBoss1Vulnerable::AIBoss1Vulnerable(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ STRING_AI_BOSS_1_VULN, ai, scr, var },
		_compRen{ nullptr },
		_input{ nullptr },
		_currentTime{ 0 },
		_origCol{},
		_redCol{ 1,0,0,1 }
	{
		if (_compAI)
		{
			_compRen = &_compAI->GetCompScript()->getComponent<ComponentRenderer>();
			_origCol = _compRen->_colour;
			_input = &_compAI->GetCompScript()->getSystemInput();
		}
	}
	AIBoss1Vulnerable::~AIBoss1Vulnerable()
	{
	}
	void AIBoss1Vulnerable::Init()
	{
		// Expose capsule here?
		_currentTime = 0;
	}
	void AIBoss1Vulnerable::Update(float dt)
	{
		_currentTime += dt;
		if (_currentTime < 5)
		{
			// If capsule is taken
			if (_currentTime < 2)
			{
				if (_compRen)
					_compRen->_colour = { 1,1,0,1 };
			}
			else
			{
				_compRen->_colour = { 0,1,1,1 };
				if (_input->GetKeyIf(IKEY_SPACE, SystemInput::PRESS))
				{
					_compAI->RunAI(STRING_AI_BOSS_1_DAMAGED);
					_compRen->_colour = _redCol;
				}
			}
		}
		else
		{
			_compRen->_colour = _origCol;
			_compAI->RunAI(STRING_AI_BOSS_1_IDLE);
		}
	}
	void AIBoss1Vulnerable::Free()
	{
	}
	void AIBoss1Vulnerable::OnCollisionEnter(size_t)
	{
	}
	void AIBoss1Vulnerable::OnCollisionExit(size_t)
	{
	}
	void AIBoss1Vulnerable::OnCollisionStay(size_t)
	{
	}
}