/******************************************************************************/
/*!
\file		ISystem.cpp
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
#include "ISystem.h"

namespace FwEngine
{
	ISystem::ISystem() : _gameObjectPool{nullptr}
	{
	}

	ISystem::ISystem(GameObjectPool* gameObjectPool) : _gameObjectPool(gameObjectPool)
	{
	}

	void ISystem::SetGameObjectPool(GameObjectPool* gameObjectPool)
	{
		_gameObjectPool = gameObjectPool;
	}

	ISystem::~ISystem()
	{
	}
}

