/******************************************************************************/
/*!
\file		GameObjectPool.h
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

#include <vector>
#include <typeinfo>
#include <typeindex>

#include "GameObject.h"

#include "Component.h"
#include "ComponentAnimation.h"
#include "ComponentCollision.h"
#include "ComponentPhysics.h"
#include "ComponentRenderer.h"
#include "ComponentTransform.h"
#include "ComponentText.h"
#include "ComponentAudio.h"
#include "ComponentEmitter.h"

namespace FwEngine
{
	class ComponentScript;

	// can use template
	// wrapper for container
	enum GAMEOBJECTCOMMAND
	{
		OBJECT_DISABLE,
		OBJECT_ENABLE,
		OBJECT_ISENABLED,
		//HASCOMPONENT,
	};

	class GameObjectPool
	{
		std::unordered_map<size_t, std::shared_ptr<void>> m_poolIndex;
		
		template <typename T>
		void AddContainer(size_t size = OBJ_MAX)
		{
			// USE THIS FOR STACK POOLS
			//m_poolIndex.try_emplace(typeid(T).hash_code(), std::shared_ptr<std::vector<T>>{ obj, [](std::vector<T>*) {} });

			// USE THIS FOR HEAP POOLS
			std::vector<T>* add = new std::vector<T>(size);
			m_poolIndex.try_emplace(typeid(T).hash_code(), add);
			if (add != GetContainerPtr<T>())
				delete add;
		}

		template <typename T>
		void FreeContainer()
		{
			T* obj = &GetContainerPtr<T>()->at(0);
			const size_t size = GetContainerPtr<T>()->size();
			for (size_t i = 0; i < size; ++i, ++obj)
			{
				obj->Free();
				obj->disable();
			}
		}

	public:
		
		typedef std::vector<GameObject> ContainerType_GameObject;
		typedef std::vector<IComponent> ContainerType_IComponent;
		typedef std::vector<ComponentAnimation> ContainerType_ComponentAnimation;
		typedef std::vector<ComponentCollision> ContainerType_ComponentCollision;
		typedef std::vector<ComponentPhysics> ContainerType_ComponentPhysics;
		typedef std::vector<ComponentRenderer> ContainerType_ComponentRenderer;
		typedef std::vector<ComponentTransform> ContainerType_ComponentTransform;
		typedef std::vector<ComponentScript> ContainerType_ComponentScript;
		typedef std::vector<ComponentText> ContainerType_ComponentText;
		typedef std::vector<ComponentAudio> ContainerType_ComponentAudio;
		typedef std::vector<ComponentEmitter> ContainerType_ComponentEmitter;
		
		//ContainerType_GameObject _gameObjectContainer;
		//ContainerType_ComponentAnimation _componentAnimationContainer;
		//ContainerType_ComponentCollision _componentCollisionContainer;
		//ContainerType_ComponentPhysics _componentPhysicsContainer;
		//ContainerType_ComponentRenderer _componentRendererContainer;
		//ContainerType_ComponentTransform _componentTransformContainer;
		//ContainerType_ComponentScript _componentScriptContainer;
		//ContainerType_ComponentText _componentTextContainer;

		template <typename T>
		std::vector<T>* GetContainerPtr()
		{
			// If Pool does not exist, return a nullptr
			try
			{
				return (std::vector<T>*)((m_poolIndex.at(typeid(T).hash_code())).get());
			}
			catch(...)
			{
				return nullptr;
			}
		}
		template <typename T>
		T* GetComponent(size_t index)
		{
			std::vector<T>* container = GetContainerPtr<T>();
			if (!container)
			{
				return nullptr;
			}
			else
			{
				try
				{
					CDEBUG_ASSERT(index < container->size(), "Accessing Component: Out of Bounds"); // <-
				}
				catch (...)
				{
					return nullptr;
				}
				return &container->at(index);
			}
		}

		template <typename T>
		T* CloneComponent(size_t destIndex, T* source)
		{
			std::vector<T>* container = GetContainerPtr<T>();
			if (!container)
			{
				return nullptr;
			}
			else
			{
				try
				{
					CDEBUG_ASSERT(destIndex < container->size(), "Accessing Component: Out of Bounds"); // <-
				}
				catch (...)
				{
					return nullptr;
				}
				T* dest = &container->at(destIndex);

				dest->setParentGameObject(GetGameObject(destIndex));
				if(source->isEnabled())
					dest->Clone(*source);
				return dest;
			}
		}



		GameObjectPool();
		GameObjectPool(size_t sz);
		~GameObjectPool();

		GameObject* GetGameObject(size_t index);

		template <typename T>
		T* FindGameObjectByName(const std::string& name)
		{
			GameObject* obj = nullptr;
			size_t size = GetContainerPtr<GameObject>()->size();
			GameObject* currObj = &GetContainerPtr<GameObject>()->at(0);
			for (size_t i = 0; i < size; ++i, ++currObj)
			{
				if (currObj->_isActive)
				{
					if (currObj->GetName() == name)
					{
						obj = currObj;
						break;
					}
				}
			}
			if (obj)
			{
				return GetComponent<T>(obj->GetIndex());
			}
			return nullptr;
		}
		
		template <typename T>
		std::vector<T*> FindGameObjectsByName(const std::string& name)
		{
			std::vector<T*> objs{};
			size_t size = GetContainerPtr<GameObject>()->size();
			GameObject* currObj = &GetContainerPtr<GameObject>()->at(0);
			for (size_t i = 0; i < size; ++i, ++currObj)
			{
				if (currObj->_isActive)
				{
					if (currObj->GetName() == name)
					{
						objs.push_back(GetComponent<T>(currObj->GetIndex()));
					}
				}
			}
			return objs;
		}

		template <typename T>
		T* FindGameObjectByTag(const std::string& tag)
		{
			ComponentTransform* obj = nullptr;
			size_t size = GetContainerPtr<ComponentTransform>()->size();
			ComponentTransform* currObj = &GetContainerPtr<ComponentTransform>()->at(0);
			for (size_t i = 0; i < size; ++i, ++currObj)
			{
				if (currObj->isEnabled())
				{
					if (currObj->_tag == tag)
					{
						obj = currObj;
						break;
					}
				}
			}
			if(obj)
			{
				return GetComponent<T>(obj->getParentGameObject()->GetIndex());
			}
			return nullptr;
		}

		template <typename T>
		std::vector<T*> FindGameObjectsByTag(const std::string& tag)
		{
			std::vector<T*> objs{};
			size_t size = GetContainerPtr<ComponentTransform>()->size();
			ComponentTransform* currObj = &GetContainerPtr<ComponentTransform>()->at(0);
			for (size_t i = 0; i < size; ++i, ++currObj)
			{
				if (currObj->isEnabled())
				{
					if (currObj->_tag == tag)
					{
						objs.push_back(GetComponent<T>(currObj->getParentGameObject()->GetIndex()));
					}
				}
			}
			return objs;
		}

		// returns index of created object
		GameObject* AddGameObject();

		// destroy game obj at index
		void DestroyGameObject(size_t index);

		void Free();

		void Swap(GameObjectPool& otherPool);

		GameObject* CloneGameObject(GameObjectPool& sourcePool, int cloneid);

		//void AttachComponent(size_t index, Component com, IComponent* comp);

		//bool DoComponent(size_t index, Component com, GAMEOBJECTCOMMAND cmd);

		//IComponent* GetComponent(size_t index, Component com);
	};
}

