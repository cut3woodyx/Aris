/*
#pragma once
#include "IScript.h"

constexpr auto STRING_SCRIPT_SCRIPT = "sample";

namespace FwEngine
{
	class ScriptSample : public IScript
	{
	protected:

	public:

		ScriptSample(ComponentScript& comp);

		virtual ~ScriptSample();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
	};

}


*/