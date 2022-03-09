#ifndef _AI_ROAMING
#define _AI_ROAMING

#include "ScriptAI.h"

namespace FwEngine
{
	class AIRoaming : public AIBase
	{
		ComponentPhysics* _compPhy;
		ComponentTransform* _playerCompTrn;

		float _distance;
		float _timePassed;
		bool _right;
		bool _hitObj;
	public:
		AIRoaming(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string var = "");
		~AIRoaming();

		virtual void Init() override;
		virtual void Update(float) override;
		virtual void Free() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		AIRoaming* Clone(AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") override
		{
			return new AIRoaming(ai, scr, str);
		}
	};
}

#endif
