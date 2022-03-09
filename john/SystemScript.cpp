/******************************************************************************/
/*!
\file		SystemScript.cpp
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
#include "SystemScript.h"
//#include "SystemInput.h"

#include "ComponentAnimation.h"
#include  "ComponentCollision.h"
#include "ComponentPhysics.h"
#include "ComponentRenderer.h"
#include  "ComponentTransform.h"
#include  "ComponentText.h"
#include "ComponentEmitter.h"

#include "SystemManager.h"
#include "GameObjectPool.h"

namespace FwEngine
{


	SystemScript::SystemScript() : SystemScript(nullptr)
	{}

	SystemScript::SystemScript(GameObjectPool* gameObjectPool)
		: ISystem(gameObjectPool), _systemManager(nullptr)
	{
	}


	SystemScript::~SystemScript()
	{
	}

	IScript* SystemScript::FindScript(int objId, std::string scriptName)
	{
		ComponentScript* scr = _systemManager->_gameObjectPool.GetComponent<ComponentScript>(objId);
		return scr->FindScript(scriptName);
	}

	void SystemScript::InitAllScripts()
	{
		ComponentScript* compScript = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<GameObject>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++compScript)
		{
			compScript->InitAllScripts();
		}

	}

	void SystemScript::OnEnterScene()
	{
		ComponentScript* compScript = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<GameObject>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++compScript)
		{
			compScript->OnEnterScene();
		}
	}

	void SystemScript::Init()
	{
		//std::cout << "SystemScript init" << std::endl;

		auto init = [&](GameObjectPool* gop)
		{
			ComponentPhysics* compPhysics = &gop->GetContainerPtr<ComponentPhysics>()->at(0);
			ComponentCollision* compCollision = &gop->GetContainerPtr<ComponentCollision>()->at(0);
			ComponentTransform* compTransform = &gop->GetContainerPtr<ComponentTransform>()->at(0);
			ComponentAnimation* compAnimation = &gop->GetContainerPtr<ComponentAnimation>()->at(0);
			ComponentRenderer* compRenderer = &gop->GetContainerPtr<ComponentRenderer>()->at(0);
			ComponentText* compText = &gop->GetContainerPtr<ComponentText>()->at(0);
			ComponentAudio* compAudio = &gop->GetContainerPtr<ComponentAudio>()->at(0);
			ComponentEmitter* compEmitter = &gop->GetContainerPtr<ComponentEmitter>()->at(0);

			ComponentScript* compScript = &gop->GetContainerPtr<ComponentScript>()->at(0);
			GameObject* gameObject = gop->GetGameObject(0);

			compScript->clearComponents();

			const size_t compSize = gop->GetContainerPtr<GameObject>()->size();
			for (size_t i = 0; i < compSize;
				++i, ++gameObject, ++compScript,
				++compPhysics, ++compCollision, ++compTransform, ++compAnimation, ++compRenderer, ++compText, ++compAudio, ++compEmitter)
			{
				compScript->setParentGameObject(gameObject);

				compScript->addComponent(compAnimation);
				compScript->addComponent(compPhysics);
				compScript->addComponent(compCollision);
				compScript->addComponent(compRenderer);
				compScript->addComponent(compTransform);
				compScript->addComponent(compText);
				compScript->addComponent(compAudio);
				compScript->addComponent(compEmitter);

				//compScript->_components.push_back(compAnimation);
				//compScript->_components.push_back(compPhysics);
				//compScript->_components.push_back(compCollision);
				//compScript->_components.push_back(compRenderer);
				//compScript->_components.push_back(compTransform);
				//compScript->_components.push_back(compText);

				compScript->_systemManager = _systemManager;
			}
		};

		init(_gameObjectPool);
		for (GameObjectPool* gop : _otherPools)
		{
			init(gop);
		}

	}

	void SystemScript::Update(float dt)
	{
		//std::cout << "SystemScript update" << std::endl;
		ComponentScript* compScript = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(0);
		GameObject* gameObject = _gameObjectPool->GetGameObject(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentScript>()->size();
		for (size_t i = 0; i < compSize;
			++i,++gameObject, ++compScript)
		{
			if (gameObject->_isActive && compScript->isEnabled())
			{
				compScript->Update(dt);
				//compScript->_GetComponent = functionGetComponent;
			}
		}
	}

	void SystemScript::UpdateMenu(float dt)
	{
		ComponentScript* compScript = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(0);
		GameObject* gameObject = _gameObjectPool->GetGameObject(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentScript>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++gameObject, ++compScript)
		{
			if (gameObject->_isActive && compScript->isEnabled() && compScript->_isMenu)
			{
				compScript->Update(dt);
				//compScript->_GetComponent = functionGetComponent;
			}
		}
	}


	void SystemScript::Unload()
	{
		ComponentScript* compScript = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentScript>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++compScript)
		{
			compScript->Unload();
		}
	}

	void SystemScript::Free()
	{
		//std::cout << "SystemScript free" << std::endl;
		ComponentScript* compScript = &_gameObjectPool->GetContainerPtr<ComponentScript>()->at(0);
		//GameObject* gameObject = _gameObjectPool->GetGameObject(0);

		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentScript>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++compScript)
		{
			compScript->disable();
			compScript->Free();
		}

		_otherPools.clear();

		_sms.Free();
	}

	void SystemScript::Destroy()
	{
		//std::cout << "SystemScript destroy" << std::endl;
	}
}
