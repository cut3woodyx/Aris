#pragma once
#include "IScript.h"

constexpr auto STRING_SCRIPT_TEMPERATURE = "temperature";

namespace FwEngine
{
	class ScriptTemperature : public IScript
	{
		GameObjectPool& _objectPool;
	public:
		float _modifier;

		ScriptTemperature(ComponentScript& comp);

		virtual ~ScriptTemperature();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void PushVars() override;
	};

}
