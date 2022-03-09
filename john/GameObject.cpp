/******************************************************************************/
/*!
\file		GameObject.cpp
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
#include "GameObject.h"

namespace FwEngine
{
	GameObject::GameObject() :
		_objectPool(nullptr), _object_index((size_t)-1), _name{},
		_isActive(false)//, _position(0,0)
	{
	}

	GameObject::GameObject(GameObjectPool* objectPool, size_t object_index, std::string name) :
		_objectPool(objectPool), _object_index(object_index), _name{ name },
		_isActive(false)//, _position(0, 0)
	{
	}

	GameObject::~GameObject()
	{
	}

	//void GameObject::init()
	//{
	//}

	//void GameObject::update(float dt)
	//{
	//}

	//void GameObject::free()
	//{
	//}

	//void GameObject::destroy()
	//{
	//}

	void GameObject::Free()
	{
	}

	GameObjectPool * GameObject::GetParentObjectPool()
	{
		return _objectPool;
	}

	size_t GameObject::GetIndex()
	{
		return _object_index;
	}


}
