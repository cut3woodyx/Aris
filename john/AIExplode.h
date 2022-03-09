#ifndef _AI_EXPLODE
#define _AI_EXPLODE

#include "ScriptAI.h"

namespace FwEngine
{
	class AIExplode : public AIBase
	{
		ComponentPhysics* _compPhy;
		ComponentRenderer* _compRenderer;
		ComponentPhysics* _playerCompPhy;
		ComponentCollision* _playerCompCol;

		float _explodeDistance;
		bool _toExplode;
		float _timeToExplode;
		float _timePassed;

		float _knockBackSpeed;

		bool _hitPlayer;
	public:
		AIExplode(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIExplode();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIExplode* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIExplode(ai, scr, str);
		}
	};
}

#endif

