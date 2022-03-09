#ifndef _AI_BOSS_1_IDLE
#define _AI_BOSS_1_IDLE

#include "ScriptAI.h"

namespace FwEngine
{
	const std::string STRING_AI_BOSS_1_IDLE = "Boss1_Idle";
	class AIBoss1Idle : public AIBase
	{
		//ComponentAnimation* _compAnim;
		float _idleTime;
		float _currentTime;
	public:
		int _bossPhase;

		AIBoss1Idle(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIBoss1Idle();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIBoss1Idle* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIBoss1Idle(ai, scr, str);
		}
	};
}

#endif

