/******************************************************************************/
/*!
\file		IScript.h
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

#include "ComponentScript.h"
#include "ComponentAnimation.h"
#include  "ComponentCollision.h"
#include "ComponentPhysics.h"
#include "ComponentRenderer.h"
#include  "ComponentTransform.h"
#include  "ComponentText.h"
#include "Component.h"
#include "ForceManager.h"
#include "SystemInput.h"

#include <map>


namespace FwEngine
{
	class IScript
	{
	protected:
		ComponentScript& _components;
	public:
		// Variables to be accessed by Editor
		std::vector<std::pair<std::string, float*>> _floatVars;
		std::vector<std::pair<std::string, int*>> _intVars;
		std::vector<std::pair<std::string, std::string*>> _stringVars;
		std::vector<std::pair<std::string, bool*>> _boolVars;

		// Variables obtained from reading file
		// Always in order of Float -> Int -> Strings -> Bool
		std::vector<std::string> _originalVars;

		std::string _name;
		IScript(std::string name, ComponentScript& comp) : _name{ name }, _components(comp) {}
		virtual ~IScript() = default;

		virtual void Init() {}
		virtual void Update(float) {}
		virtual void Unload() {}
		virtual void Free() {}
		virtual void Destroy() {}

		virtual void OnCollisionEnter(size_t) {}
		virtual void OnCollisionExit(size_t) {}
		virtual void OnCollisionStay(size_t) {}

		virtual void OnEnterScn() {}

		void AddFloatVar(std::string name, float* var) { _floatVars.push_back({ name, var }); }
		void AddIntVar(std::string name, int* var) { _intVars.push_back({ name, var }); }
		void AddStringVar(std::string name, std::string* var) { _stringVars.push_back({ name, var }); }
		void AddBoolVar(std::string name, bool* var) { _boolVars.push_back({ name, var }); }
		virtual void PushVars() {}

		//virtual void OnTriggerEnter(int id) = 0;
		//virtual void OnTriggerExit(int id) = 0;
		//virtual void OnTriggerStay(int id) = 0;

	};

	class Timer
	{
		clock_t _initTime;
		int _setTime;
		std::string clockToString(clock_t time);
	public:
		Timer(int time);
		clock_t getElapsedTime();
		std::string getElapsedTime_s();
		std::string getRemainingTime_s();
		bool checkTrigger();
		void reset();
		//void pause();
		//void start();
	};

	class ScriptMessageObject
	{
	public:
		// can add stats if needed?
	};

	class ScriptMessage_Player : public ScriptMessageObject
	{
	public:
		bool movementParticle;
		FwMath::Vector3D velocity;

		ScriptMessage_Player() : movementParticle(), velocity() {}
	};

	class ScriptMessagingSystem // SMS (lol)
	{
		// an address message pair of string and ScriptMessageObject*
		std::map<std::string, ScriptMessageObject*> _messageBox;
	public:
		ScriptMessagingSystem();
		~ScriptMessagingSystem();
		
		void Free();

		std::map<std::string, ScriptMessageObject*>::iterator FindAddress(std::string addressName);

		void RegisterAddress(std::string addressName);

		ScriptMessageObject* RegisterMessage(std::string addressName, ScriptMessageObject* message);

		ScriptMessageObject* RegisterListener(std::string addressName);

		template<typename ScriptMessageObjectT>
		ScriptMessageObject* TryRegister(std::string addressName)
		{
			ScriptMessageObject* mso = RegisterListener("");
			// if not found, allocate new
			if (!mso)
			{
				mso = new ScriptMessageObjectT;
				RegisterMessage("", mso);
			}
			return mso;
		}

	};

}
