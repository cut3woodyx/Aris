/******************************************************************************/
/*!
\file		ScriptDialogue.h
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Vector.h"
#include "IScript.h"

constexpr auto STRING_SCRIPT_DIALOGUE = "dialogue";

namespace FwEngine
{
	class ScriptDialogue : public IScript
	{
	protected:
		GameObjectPool& objectPool;
		ComponentTransform& comTrans;
		ComponentText& comText;
		SystemInput& inp;
		SystemGraphics& gfx;
		SystemManager& sysMgr;
		ComponentTransform* _textBackground;
		ComponentRenderer* _renderBackground;
		ComponentTransform* _playerTrans;
		ComponentCollision* _playerCol;
		std::vector<std::string> _dialogue;
		size_t textIndex;

		bool _spawnText;
		std::string _zoneName;

	public:
		std::string _filename;
		bool _once;

		ScriptDialogue(ComponentScript& comp);

		virtual ~ScriptDialogue();

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
