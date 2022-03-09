#ifndef _AI_BOSS_1_VULN
#define _AI_BOSS_1_VULN

#include "ScriptAI.h"
#include "AIBoss1Idle.h"

namespace FwEngine
{
	const std::string STRING_AI_BOSS_1_VULN = "Boss1_Vuln";
	class AIBoss1Vulnerable : public AIBase
	{
		ComponentRenderer* _compRen;
		SystemInput* _input;
		float _currentTime;

		FwMath::Vector4D _origCol;
		FwMath::Vector4D _redCol;
	public:
		AIBoss1Vulnerable(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIBoss1Vulnerable();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIBoss1Vulnerable* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIBoss1Vulnerable(ai, scr, str);
		}
	};
}

#endif