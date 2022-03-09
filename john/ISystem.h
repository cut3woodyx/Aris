/******************************************************************************/
/*!
\file		ISystem.h
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

#include "GameObjectPool.h"

namespace FwEngine
{
	class ISystem
	{
	protected:
		GameObjectPool* _gameObjectPool;

	public:
		ISystem();

		ISystem(GameObjectPool* gameObjectPool);
		void SetGameObjectPool(GameObjectPool* gameObjectPool);

		virtual ~ISystem();

		virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		virtual void Free() = 0;
		virtual void Destroy() = 0;
	};

}
