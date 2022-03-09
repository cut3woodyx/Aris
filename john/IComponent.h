/******************************************************************************/
/*!
\file		IComponent.h
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

#include "Component.h"
#include <string>


#include <algorithm> // for std::copy in clone

namespace FwEngine
{
	// few decl
	class GameObject;

	class IComponent
	{
	protected:
		bool _isEnabled;
		GameObject* _parentGameObject;
	public:
		IComponent() : _parentGameObject(nullptr), _isEnabled(false) {}
		IComponent(GameObject* parent) : _parentGameObject(parent), _isEnabled(false) {}
		virtual ~IComponent();
		void enable() { _isEnabled = true; }
		void disable() { _isEnabled = false; }
		bool isEnabled() { return _isEnabled; }
		void setEnable(bool enable) { _isEnabled = enable; }
		GameObject* getParentGameObject() { return _parentGameObject; }
		void setParentGameObject(GameObject* parentGameObject) { _parentGameObject = parentGameObject; }

		virtual void Init(ParamValueMap& paramValues) = 0;

		virtual void Clone(IComponent& sauce) = 0;

		virtual void Update(float dt) = 0;
		virtual void Free() = 0;
		virtual void Destroy() = 0;

		virtual std::pair<std::string, ParamValueMap> GetParams() = 0;
	};

}

