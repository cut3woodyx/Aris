/******************************************************************************/
/*!
\file		ScriptSpikeBox.h
\project	Aris
\author 	Primary: Wang YiDi
\par    	email: w.yidi\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "IScript.h"
#include "ScriptPlayer.h"
constexpr auto STRING_SCRIPT_SPIKE_BOX = "spike box";

namespace FwEngine
{

	class ScriptSpikeBox : public IScript
	{
	protected:
		GameObjectPool& objectPool;
		ComponentCollision& colCollision;
		ScriptPlayer* pScr;
		char side;
	public:

		ScriptSpikeBox(ComponentScript& comp);

		virtual ~ScriptSpikeBox();

		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Free() override;

		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;
	};

}
