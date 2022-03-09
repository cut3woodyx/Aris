#include "AIBoss1Attack.h"
#include "AIBoss1Idle.h"

namespace FwEngine
{
	AIBoss1Attack::AIBoss1Attack(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ STRING_AI_BOSS_1_ATTACK, ai, scr, var },
		_compTrans{ nullptr },
		_tilePlatform{ nullptr }
	{
		if (_compAI)
		{
			_compTrans = &_compAI->GetCompScript()->getComponent<ComponentTransform>();
			_tilePlatform = reinterpret_cast<ScriptTilePlatform*>(_compAI->GetCompScript()->FindScript(STRING_SCRIPT_TILEPLATFORM));
		}
	}
	AIBoss1Attack::~AIBoss1Attack()
	{
	}
	void AIBoss1Attack::Init()
	{
	}
	void AIBoss1Attack::Update(float)
	{
		if (_tilePlatform)
		{
			_compTrans->_currentPosition.x += 200.0f;
			// Break 2 kek
			_tilePlatform->BreakLeftColumn();
			_tilePlatform->BreakLeftColumn();
		}

		_compAI->RunAI(STRING_AI_BOSS_1_IDLE);
	}
	void AIBoss1Attack::Free()
	{
	}
	void AIBoss1Attack::OnCollisionEnter(size_t)
	{
	}
	void AIBoss1Attack::OnCollisionExit(size_t)
	{
	}
	void AIBoss1Attack::OnCollisionStay(size_t)
	{
	}
}