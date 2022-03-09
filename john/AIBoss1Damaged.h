#ifndef _AI_BOSS_1_DAMAGED
#define _AI_BOSS_1_DAMAGED

#include "ScriptAI.h"
#include "AIBoss1Idle.h"

namespace FwEngine
{
	const std::string STRING_AI_BOSS_1_DAMAGED = "Boss1_Damaged";
	class AIBoss1Damaged : public AIBase
	{
		ComponentRenderer* _compRen;
		std::shared_ptr<AIBoss1Idle> _bossIdle;
	public:
		AIBoss1Damaged(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIBoss1Damaged();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIBoss1Damaged* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIBoss1Damaged(ai, scr, str);
		}
	};
}

#endif