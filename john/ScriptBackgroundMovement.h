#pragma once
#include "Vector.h"
#include "IScript.h"

constexpr auto STRING_SCRIPT_BACKGROUND = "background";

namespace FwEngine
{
	class ScriptBackgroundMovement : public IScript
	{
	protected:
		GameObjectPool& objectPool;
		ComponentTransform& playerTrans;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;
		ComponentTransform* _leftBackground;
		ComponentTransform* _rightBackground;

	public:
		ScriptBackgroundMovement(ComponentScript& comp);

		virtual ~ScriptBackgroundMovement();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

	};

}
