#ifndef _AI_BOSS_1_CHARGE
#define _AI_BOSS_1_CHARGE

#include "ScriptAI.h"
#include "SystemInput.h"

namespace FwEngine
{
	const std::string STRING_AI_BOSS_1_CHARGE = "Boss1_Charge";
	class AIBoss1Charge : public AIBase
	{
		ComponentTransform* _compTrans;
		SystemInput* _input;
		float _currentTime;
		int _metalEaten;
	public:
		AIBoss1Charge(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIBoss1Charge();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIBoss1Charge* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIBoss1Charge(ai, scr, str);
		}
	};
}

#endif