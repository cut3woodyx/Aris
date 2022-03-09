/******************************************************************************/
/*!
\file		GameObject.h
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

#include "GameObject.h"
#include "GameObjectPool.h"

#include "Component.h"
#include "ComponentScript.h"

#include <string>
#include <iostream>

#include <unordered_map>

#include "SystemCustomSerialization.h"
#include "SystemGraphics.h"

#include "SystemObservable.h"
#include "SystemCustomMessages.h"

namespace FwEngine
{
	class GameObjectFactory
	{
		template <typename T>
		struct TypedObj
		{
			typedef T Arg;
		};

		GameObjectPool* _currentPool;

		SystemGraphics* _systemGraphics;

		//std::string getFirst(ParamValueMap& paramValues, std::string key);

		void BuildComponent(GameObject* newObj, ParamValueMap& paramValues, std::string componentName, bool isFab = false);
		//void buildCollisionComponent(GameObject* newObj, ParamValueMap& paramValues);
		//void buildPhysicsComponent(GameObject* newObj, ParamValueMap& paramValues);

		std::unordered_map<std::string, std::function<IComponent* (size_t)>> m_map;

		IComponent* GetObjFromMap(std::string name, size_t index)
		{
			try
			{
				return m_map.at(name)(index);
			}
			catch (...)
			{
				return nullptr;
			}
		}

		template <typename T>
		IComponent* GetObj(size_t index)
		{
			return _currentPool->GetComponent<T>(index);
		}

		template <typename T>
		void AddNameToMap (std::string name)
		{
			m_map.try_emplace(name,
			std::bind(&GameObjectFactory::GetObj<T>, this, std::placeholders::_1));
		}

	public:

		SystemListener _gameObjListener;

		std::string _cerealise;

		std::string& CerealiseGameObjects();

		GameObjectFactory();
		~GameObjectFactory();

		void SetCurrentPool(GameObjectPool* currentPool);
		void SetSystemGraphics(SystemGraphics* gfx);

		void CreateSerialisedGameObjects(std::string fileName, bool isFab = false);

		GameObject* CreateSingleSerializedGameObject(std::string fileName, bool isFab = false);

		//
		GameObject* CreateGameObject_Test1(
			FwMath::Vector3D position = FwMath::Vector3D(0, 0, 0), 
			FwMath::Vector3D velocity = FwMath::Vector3D(0, 0, 0),
			std::string path = "Resources/Sprites/player_test.png"
			);

		//
		GameObject* CreateGameObject_Empty(
			FwMath::Vector3D scale = FwMath::Vector3D(0, 0, 0),
			FwMath::Vector3D position = FwMath::Vector3D(0, 0, 0)
			);

		//
		ComponentRenderer* GameObject_AttachComponent_Renderer(
			size_t objId,
			std::string path,
			int layer = 0
		);

		//
		ComponentText* GameObject_AttachComponent_Text(
			size_t objId,
			std::string text
		);

		//
		ComponentScript* GameObject_AttachComponent_Script(
			size_t objId,
			std::string script
		);


		// Event Messaging Testing
		void CreateObject(InputMessage&);

	};

}

