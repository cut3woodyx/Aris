/******************************************************************************/
/*!
\file		GameObjectPool.cpp
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
#include "GameObjectPool.h"

#include "ComponentScript.h"

namespace FwEngine
{
	GameObjectPool::GameObjectPool()
	{
		//_gameObjectContainer = ContainerType_GameObject(OBJ_MAX);
		//_componentAnimationContainer = ContainerType_ComponentAnimation(OBJ_MAX);
		//_componentCollisionContainer = ContainerType_ComponentCollision(OBJ_MAX);
		//_componentPhysicsContainer = ContainerType_ComponentPhysics(OBJ_MAX);
		//_componentRendererContainer = ContainerType_ComponentRenderer(OBJ_MAX);
		//_componentTransformContainer = ContainerType_ComponentTransform(OBJ_MAX);
		//_componentScriptContainer = ContainerType_ComponentScript(OBJ_MAX);
		//_componentTextContainer = ContainerType_ComponentText(OBJ_MAX);

		AddContainer<GameObject>(); //(new ContainerType_GameObject(OBJ_MAX));
		AddContainer<ComponentAnimation>(); //(new ContainerType_ComponentAnimation(OBJ_MAX));
		AddContainer<ComponentCollision>(); //(new ContainerType_ComponentCollision(OBJ_MAX));
		AddContainer<ComponentPhysics>(); //(new ContainerType_ComponentPhysics(OBJ_MAX));
		AddContainer<ComponentRenderer>(); //(new ContainerType_ComponentRenderer(OBJ_MAX));
		AddContainer<ComponentTransform>(); //(new ContainerType_ComponentTransform(OBJ_MAX));
		AddContainer<ComponentScript>(); //(new ContainerType_ComponentScript(OBJ_MAX));
		AddContainer<ComponentText>(); //(new ContainerType_ComponentText(OBJ_MAX));
		AddContainer<ComponentAudio>(); //(new ContainerType_ComponentAudio(OBJ_MAX));
		AddContainer<ComponentEmitter>();
		//AddContainer<ComponentUI>(); //(new ContainerType_ComponentUI(OBJ_MAX));
		//AddContainer(new ContainerType_ComponentAnimation(1));
		//AddContainer(&_componentAnimationContainer);
		/* example 
			ContainerType_ComponentAnimation* a = GetContainerPtr<ComponentAnimation>();
			a->at(0).isEnabled();
			ContainerType_ComponentPhysics* b = GetContainerPtr<ComponentPhysics>();
			if (!b)
			{
				std::cout << "FAILURE TO OBTAIN POINTER" << std::endl;
			}
		*/
	}

	GameObjectPool::GameObjectPool(size_t sz)
	{
		AddContainer<GameObject>(sz); 
		AddContainer<ComponentAnimation>(sz);
		AddContainer<ComponentCollision>(sz);
		AddContainer<ComponentPhysics>(sz); 
		AddContainer<ComponentRenderer>(sz); 
		AddContainer<ComponentTransform>(sz);
		AddContainer<ComponentScript>(sz); 
		AddContainer<ComponentText>(sz); 
		AddContainer<ComponentAudio>(sz);
		AddContainer<ComponentEmitter>(sz);
	}

	GameObjectPool::~GameObjectPool()
	{
	}

	GameObject* GameObjectPool::GetGameObject(size_t index)
	{
		return GetComponent<GameObject>(index);
	}

	GameObject* GameObjectPool::AddGameObject()
	{
		ContainerType_GameObject& cont = *GetContainerPtr<GameObject>();
		//gameObjectContainer.push_back(gamObj);
		GameObject* currentObj = &cont.at(0);
		for (int i = 0; i < cont.size(); ++i, ++currentObj)
		{
			if (!currentObj->_isActive)
			{
				*currentObj = GameObject(this, i);
				currentObj->_isActive = true;
				return currentObj;
			}
		}

		// obj not created ***
		return nullptr;
	}

	void GameObjectPool::DestroyGameObject(size_t index)
	{
		GetContainerPtr<GameObject>()->at(index)._isActive = false;

		//_gameObjectContainer.at(index)._isActive = false;
	}

	void GameObjectPool::Free()
	{
		FreeContainer<GameObject>();
		FreeContainer<ComponentAnimation>();
		FreeContainer<ComponentCollision>();
		FreeContainer<ComponentPhysics>();
		FreeContainer<ComponentRenderer>();
		FreeContainer<ComponentTransform>();
		FreeContainer<ComponentScript>();
		FreeContainer<ComponentText>();
		FreeContainer<ComponentAudio>();
		FreeContainer<ComponentEmitter>();
		//FreeContainer<ComponentUI>();
	}

	void GameObjectPool::Swap(GameObjectPool& otherPool)
	{
		//std::swap(m_poolIndex, otherPool.m_poolIndex);
		//m_poolIndex.swap(otherPool.m_poolIndex);

		std::unordered_map<size_t, std::shared_ptr<void>>::iterator thisItr = m_poolIndex.begin();
		//std::unordered_map<size_t, std::shared_ptr<void>>::iterator otherItr = otherPool.m_poolIndex.begin();

		while (thisItr != m_poolIndex.end())
		{
			std::unordered_map<size_t, std::shared_ptr<void>>::iterator otherItr = otherPool.m_poolIndex.find((*thisItr).first);
			//std::swap(otherItr->second, thisItr->second);
			if (otherItr != otherPool.m_poolIndex.end())
				otherItr->second.swap(thisItr->second);
			++thisItr;
		}
	}

	GameObject* GameObjectPool::CloneGameObject(GameObjectPool& sourcePool, int cloneid)
	{
		GameObject* newObject = AddGameObject();

		if (newObject == nullptr)
		{
			return nullptr;
		}

		size_t newObjectId = newObject->GetIndex();

		CloneComponent<ComponentAnimation>(newObjectId, sourcePool.GetComponent<ComponentAnimation>(cloneid));
		CloneComponent<ComponentCollision>(newObjectId, sourcePool.GetComponent<ComponentCollision>(cloneid));
		CloneComponent<ComponentPhysics>(newObjectId, sourcePool.GetComponent<ComponentPhysics>(cloneid));
		CloneComponent<ComponentRenderer>(newObjectId, sourcePool.GetComponent<ComponentRenderer>(cloneid));
		CloneComponent<ComponentTransform>(newObjectId, sourcePool.GetComponent<ComponentTransform>(cloneid));
		CloneComponent<ComponentScript>(newObjectId, sourcePool.GetComponent<ComponentScript>(cloneid));
		CloneComponent<ComponentText>(newObjectId, sourcePool.GetComponent<ComponentText>(cloneid));
		CloneComponent<ComponentAudio>(newObjectId, sourcePool.GetComponent<ComponentAudio>(cloneid));
		CloneComponent<ComponentEmitter>(newObjectId, sourcePool.GetComponent<ComponentEmitter>(cloneid));

		return newObject;
	}

	//void GameObjectPool::AttachComponent(size_t index, Component com, IComponent* comp)
	//{
	//	switch (com)
	//	{
	//	case (COMPONENT_ANIMATION):
	//		_componentAnimationContainer.at(index) = *(static_cast<ComponentAnimation*>(comp));
	//		break;
	//	case (COMPONENT_COLLISION):
	//		_componentCollisionContainer.at(index) = *(static_cast<ComponentCollision*>(comp));
	//		break;
	//	case (COMPONENT_PHYSICS):
	//		_componentPhysicsContainer.at(index) = *(static_cast<ComponentPhysics*>(comp));
	//		break;
	//	case (COMPONENT_RENDERER):
	//		_componentRendererContainer.at(index) = *(static_cast<ComponentRenderer*>(comp));
	//		break;
	//	case (COMPONENT_TRANSFORM):
	//		_componentTransformContainer.at(index) = *(static_cast<ComponentTransform*>(comp));
	//		break;
	//	case (COMPONENT_SCRIPT):
	//		_componentScriptContainer.at(index) = *(static_cast<ComponentScript*>(comp));
	//		break;
	//	case (COMPONENT_TEXT):
	//		_componentTextContainer.at(index) = *(static_cast<ComponentText*>(comp));
	//		break;
	//	default:
	//		throw;
	//	}
	//}

	//bool GameObjectPool::DoComponent(size_t index, Component com, GAMEOBJECTCOMMAND cmd)
	//{
	//	IComponent* comp = GetComponent(index, com);
	//	switch (cmd)
	//	{
	//	case OBJECT_DISABLE:
	//		comp->disable();
	//		break;
	//	case OBJECT_ENABLE:
	//		comp->enable();
	//		break;
	//	case OBJECT_ISENABLED:
	//		return comp->isEnabled();
	//		break;
	//	}
	//	return true;
	//}

	//IComponent * GameObjectPool::GetComponent(size_t index, Component com)
	//{
	//	IComponent* comp = nullptr;
	//	switch (com)
	//	{
	//	case (COMPONENT_ANIMATION):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentAnimationContainer.at(index))
	//			);
	//		break;
	//	case (COMPONENT_COLLISION):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentCollisionContainer.at(index))
	//			);
	//		break;
	//	case (COMPONENT_PHYSICS):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentPhysicsContainer.at(index))
	//			);
	//		break;
	//	case (COMPONENT_RENDERER):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentRendererContainer.at(index))
	//			);
	//		break;
	//	case (COMPONENT_TRANSFORM):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentTransformContainer.at(index))
	//			);
	//		break;
	//	case (COMPONENT_SCRIPT):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentScriptContainer.at(index))
	//			);
	//		break;
	//	case (COMPONENT_TEXT):
	//		comp = dynamic_cast<IComponent*>(
	//			&(_componentTextContainer.at(index))
	//			);
	//		break;
	//	default:
	//		throw;
	//	}
	//	return comp;
	//}

	

}
