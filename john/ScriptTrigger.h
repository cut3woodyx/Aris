#include "IScript.h"

#pragma once

constexpr auto STRING_SCRIPT_TRIGGER = "scriptTrigger";
constexpr float CAMERA_SHAKE_TIMER = 2.0f;

namespace FwEngine
{
	class ScriptTrigger : public IScript
	{
	protected:
		SystemGraphics& _sysGraphics;
		GameObjectPool& _objectPool;

		float _timer;
	public:
		ScriptTrigger(ComponentScript& comp);
		virtual ~ScriptTrigger();

		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void Free() override;
		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
	};
}


