/******************************************************************************/
/*!
\file		ComponentScript.h
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

#pragma once

#include "Math.h"
#include "IComponent.h"
#include "Component.h"
#include "GameObjectPool.h"
#include <vector>
#include <list>
#include <unordered_map>

#include "SystemInput.h"
#include "SystemGraphics.h"


namespace FwEngine
{
	constexpr auto STRING_COMPONENT_SCRIPT = "script";
	typedef FwMath::Vector3D Vector3D;

	class IScript;
	class SystemManager;
	class ScriptGongChang;
	class SystemScript;

	class ComponentScript : public IComponent
	{
		//typedef IComponent* (*FunctionGetComponent)(size_t index, Component com);
		//FunctionGetComponent _GetComponent;

		std::list<IScript*> _scripts;

		//std::vector<IComponent*> _components;
		std::unordered_map<size_t, void*> _components;

		SystemManager* _systemManager;

	public:
		bool _isMenu;

		ComponentScript();
		ComponentScript(GameObject* parent);
		~ComponentScript();

		template <typename T>
		T& getComponent()
		{
			try
			{
				//_components.at(typeid(T).hash_code()).get();
				T* comp = (T*)((_components.at(typeid(T).hash_code())));
				return *comp;
			}
			catch (...)
			{
				throw; //
			}
		}

		//IComponent& getObjectComponent(int index, Component component);
		template <typename T>
		T& getObjectComponent(int index)
		{
			//GameObject* obj = _parentGameObject->GetParentObjectPool->GetIndex(index);

			GameObjectPool* gop = _parentGameObject->GetParentObjectPool();

			T& comp = *(gop->GetComponent<T>(index));

			return comp;
		}

		template <typename T>
		void addComponent(T* comp)
		{
			_components.try_emplace(typeid(T).hash_code(), comp);
		}

		SystemInput& getSystemInput();
		SystemGraphics& getSystemGraphics();
		SystemManager& getSystemManager();
		SystemScript& getSystemScript();
		FwMath::Vector3D getCursorPosition();

		void clearComponents()
		{
			_components.clear();
		}

		void InitAllScripts();

		virtual void OnCollisionEnter(size_t id);
		virtual void OnCollisionExit(size_t id);
		virtual void OnCollisionStay(size_t id);

		virtual void OnEnterScene();

		//void OnTriggerEnter(int id);
		//void OnTriggerExit(int id);
		//void OnTriggerStay(int id);

		void AttachScript(std::string name);
		IScript* DeattachScript(std::string name);
		void DeleteScript(std::string name);

		IScript* FindScript(std::string name);

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Unload();
		void Free() override;
		void Destroy() override;

		const std::list<IScript*> GetListOfScripts() {return _scripts;}

		virtual std::pair<std::string, ParamValueMap> GetParams();

		friend class SystemScript;
	};

}