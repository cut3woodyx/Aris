/******************************************************************************/
/*!
\file		ComponentScript.cpp
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <iostream>

#include  "ComponentScript.h"
#include "ComponentAnimation.h"
#include  "ComponentCollision.h"
#include "ComponentPhysics.h"
#include "ComponentRenderer.h"
#include  "ComponentTransform.h"
#include  "ComponentText.h"
#include "GameObject.h"
#include "IScript.h"

#include "ScriptFactory.h"

//#include "ScriptBackgroundMovement.h"

//#include "Blast.h"
//
//#include "ScriptPlayer.h"
//#include "ScriptPlayerUI.h"
//#include "ScriptTestObjectOne.h"
//#include "ScriptTrigger.h"
//#include "ScriptMetal.h"
//#include "ScriptStart.h"
//
//#include "ScriptMainMenu.h"
//#include "ScriptMenuUIButton.h"
//#include "ScriptDebug.h"
//#include "ScriptUIText.h"
//#include "ScriptKillZone.h"
//#include "ScriptMagneticLight.h"

#include "SystemManager.h"

constexpr auto STRING_SCRIPT = "script";

constexpr auto STRING_MENUOBJECT = "menuobject";

namespace FwEngine
{
	ComponentScript::ComponentScript() : ComponentScript(nullptr)
	{
	}

	ComponentScript::ComponentScript(GameObject* parent) : IComponent(parent), _scripts(), _systemManager(nullptr), _isMenu(false)
	{
	}


	ComponentScript::~ComponentScript()
	{
		for (IScript*& scr :_scripts)
		{
			if (scr != nullptr)
			{
				delete scr;
				scr = nullptr;
			}
		}
		_scripts.clear();
	}

	//IComponent& ComponentScript::getComponent(Component component)
	//{
	//	return *_components.at(component);
	//}

	SystemInput& ComponentScript::getSystemInput()
	{
		return _systemManager->_systemInput;
	}

	SystemGraphics& ComponentScript::getSystemGraphics()
	{
		return _systemManager->_systemGraphics;
	}

	SystemManager& ComponentScript::getSystemManager()
	{
		return *_systemManager;
	}

	SystemScript& ComponentScript::getSystemScript()
	{
		return _systemManager->_systemScript;
	}

	FwMath::Vector3D ComponentScript::getCursorPosition()
	{
		return _systemManager->_cursorPosition;
	}

	void ComponentScript::InitAllScripts()
	{
		for (IScript* scr : _scripts)
		{
			scr->Init();
		}
	}

	void ComponentScript::OnCollisionEnter(size_t id)
	{
		for (IScript* scr : _scripts)
		{
			scr->OnCollisionEnter(id);
		}
	}

	void ComponentScript::OnCollisionExit(size_t id)
	{
		for (IScript* scr : _scripts)
		{
			scr->OnCollisionExit(id);
		}
	}

	void ComponentScript::OnCollisionStay(size_t id)
	{
		for (IScript* scr : _scripts)
		{
			scr->OnCollisionStay(id);
		}
	}

	void ComponentScript::OnEnterScene()
	{
		for (IScript* scr : _scripts)
		{
			scr->OnEnterScn();
		}
	}

	void ComponentScript::AttachScript(std::string name)
	{
		ScriptGongChang _scriptFactory(*this);

		IScript* newScript = _scriptFactory.CreateType(name, *this);

		if(newScript)
			_scripts.push_back(newScript);
	}

	IScript* ComponentScript::DeattachScript(std::string name)
	{
		auto itr = _scripts.begin();
		auto itrEnd = _scripts.end();
		while (itr != itrEnd)
		{
			IScript* script = *itr;
			if ((*itr)->_name._Equal(name))
			{
				_scripts.erase(itr);
				return script;
			}
			++itr;
		}
		return nullptr;
	}

	void ComponentScript::DeleteScript(std::string name)
	{
		delete DeattachScript(name);
	}

	IScript* ComponentScript::FindScript(std::string name)
	{
		auto itr = _scripts.begin();
		auto itrEnd = _scripts.end();
		while (itr != itrEnd)
		{
			IScript* script = *itr;
			if ((*itr)->_name._Equal(name))
			{
				return script;
			}
			++itr;
		}
		return nullptr;
	}

	void ComponentScript::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		_isMenu = GetFirst(paramValues, std::string(STRING_MENUOBJECT))._Equal("true");

		int size = (int)paramValues.size();
		for (int i = 0; i < size; ++i)
		{
			std::stringstream paramString(GetFirst(paramValues,
				std::string(STRING_SCRIPT).append(std::to_string(i))));
			std::string script;

			std::vector<std::string> paramTokens;
			std::string check;
			
			std::getline(paramString, script, ' ');

			while (std::getline(paramString, check, ' '))
				paramTokens.push_back(check);

			ScriptGongChang _scriptFactory(*this);

			IScript* newScript = _scriptFactory.CreateType(script, *this);

			if (newScript) 
			{
				//newScript->Init();
				_scripts.push_back(newScript);

				newScript->_originalVars.clear();

				for (std::string token : paramTokens)
					newScript->_originalVars.push_back(token);

				newScript->PushVars();
			}

		}
	}

	void ComponentScript::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentScript& source = reinterpret_cast<ComponentScript&>(sauce);

		_isMenu = source._isMenu;

		std::copy(source._scripts.begin(), source._scripts.end(), _scripts.begin());
	}

	void ComponentScript::Update(float dt)
	{

		for (IScript* scr : _scripts)
		{
			scr->Update(dt);
		}

	}

	void ComponentScript::Unload()
	{
		for (IScript*& scr : _scripts)
		{
			if (scr != nullptr)
			{
				scr->Unload();
			}
		}
	}

	void ComponentScript::Free()
	{
		for (IScript*& scr : _scripts)
		{
			if (scr != nullptr)
			{
				scr->Free();
				delete scr;
				scr = nullptr;
			}
		}
		_scripts.clear();
	}

	void ComponentScript::Destroy()
	{
		for (IScript*& scr : _scripts)
		{
			if (scr != nullptr)
			{
				delete scr;
				scr = nullptr;
			}
		}
		_scripts.clear();
	}

	std::pair<std::string, ParamValueMap> ComponentScript::GetParams()
	{
		ParamValueMap params;
		int i = 0;
		for (IScript* script : _scripts)
		{
			if (script != nullptr)
			{
				std::stringstream vall;
				vall << script->_name;

				for (auto p : script->_floatVars)
					vall << " " << std::to_string(*p.second);

				for (auto p : script->_intVars)
					vall << " " << std::to_string(*p.second);

				for (auto p : script->_stringVars)
					vall << " " << *p.second;

				for (auto p : script->_boolVars)
					vall << " " << std::to_string(*p.second);

				params.emplace(std::string(STRING_SCRIPT).append(std::to_string(i)), vall.str());
			}
			++i;
		}
		return { STRING_COMPONENT_SCRIPT, params };
	}

}
