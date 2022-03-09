#ifndef _AI_BOSS_1_ATTACK
#define _AI_BOSS_1_ATTACK

#include "ScriptAI.h"
#include "ScriptTilePlatform.h"

namespace FwEngine
{
	const std::string STRING_AI_BOSS_1_ATTACK = "Boss1_Attack";
	class AIBoss1Attack : public AIBase
	{
		ComponentTransform* _compTrans;
		ScriptTilePlatform* _tilePlatform;
	public:
		AIBoss1Attack(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIBoss1Attack();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIBoss1Attack* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIBoss1Attack(ai, scr, str);
		}
	};
}

#endif

