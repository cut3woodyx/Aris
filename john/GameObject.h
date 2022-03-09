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

#include "IComponent.h"
#include "Component.h"
//#include "Math_Vector2D.h"

namespace FwEngine
{
	// fwd declare
	class GameObjectPool;

	class GameObject
	{
		GameObjectPool* _objectPool;
		size_t _object_index;
		//IComponent* componentList[COMPONENT_END] = {nullptr};
		// int _object_id; // can just use pointer to id
		std::string _name;
	public:
		bool _isActive;
		//FwMath::Vec2 _position;
		GameObject();
		GameObject(GameObjectPool* objectPool, size_t object_index, std::string name = "");
		~GameObject();

		void enable() { _isActive = true; }
		void disable() { _isActive = false; }

		std::string GetName() { return _name; }
		void SetName(std::string name) { _name = name; }

		//void init();
		//void update(float dt);
		void Free();
		//void destroy();

		GameObjectPool* GetParentObjectPool();
		size_t GetIndex();

	};

}

