#include "AIBoss1VField.h"
#include "AIBoss1Idle.h"

namespace FwEngine
{
	AIBoss1VField::AIBoss1VField(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ STRING_AI_BOSS_1_VFIELD, ai, scr, var },
		_compRen{ nullptr },
		_currentTime{ 0 }
	{
		if (_compAI)
		{
			_compRen = &_compAI->GetCompScript()->getComponent<ComponentRenderer>();
		}
	}

	AIBoss1VField::~AIBoss1VField()
	{
	}

	void AIBoss1VField::Init()
	{
		_currentTime = 0;
		if (_compRen)
			_compRen->_colour = { 0,1,0,1 };
	}

	void AIBoss1VField::Update(float dt)
	{
		_currentTime += dt;
		if (_currentTime > 2)
		{
			if (_compRen)
				_compRen->_colour = { 1,1,1,1 };
			_compAI->RunAI(STRING_AI_BOSS_1_IDLE);
		}
	}

	void AIBoss1VField::Free()
	{
	}

	void AIBoss1VField::OnCollisionEnter(size_t)
	{
	}

	void AIBoss1VField::OnCollisionExit(size_t)
	{
	}

	void AIBoss1VField::OnCollisionStay(size_t)
	{
	}
}