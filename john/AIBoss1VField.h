#ifndef _AI_BOSS_1_VFIELD
#define _AI_BOSS_1_VFIELD

#include "ScriptAI.h"

namespace FwEngine
{
	const std::string STRING_AI_BOSS_1_VFIELD = "Boss1_VField";
	class AIBoss1VField : public AIBase
	{
		ComponentRenderer* _compRen;
		float _currentTime;
	public:
		AIBoss1VField(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIBoss1VField();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIBoss1VField* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIBoss1VField(ai, scr, str);
		}
	};
}

#endif