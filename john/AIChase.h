#ifndef _AI_CHASE
#define _AT_CHASE

#include "ScriptAI.h"


namespace FwEngine
{
	class AIChase : public AIBase
	{
		ComponentPhysics* _compPhy;
		ComponentPhysics* _playerCompPhy;

		bool _chasing;
		float _chaseSpeed;
		float _chaseDistance;
		float _explodeDistance;

	public:
		AIChase(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIChase();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIChase* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIChase(ai, scr, str);
		}
	};
}

#endif
