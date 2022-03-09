/******************************************************************************/
/*!
\file		SystemObservable.cpp
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "SystemObservable.h"
#include "DebugDiagnostic.h"
#include <algorithm>

FwEngine::SystemObservable::SystemObservable() : _id{ "\n" }
{
}

FwEngine::SystemObservable::SystemObservable(std::string id) : _id {id}
{
}

void FwEngine::SystemObservable::Register(const std::string msg_ID, SystemListener& listener)
{
	_observerMap.insert(ObserverMap::value_type(msg_ID, listener));
	listener._allRegisteredObs.insert(this);
}

void FwEngine::SystemObservable::Unregister(const std::string msg_ID, SystemListener& listener)
{
	std::pair<std::multimap<std::string, SystemListener&>::iterator,
		std::multimap<std::string, SystemListener&>::iterator> range
		= _observerMap.equal_range(msg_ID);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (&it->second == &listener)
		{
			_observerMap.erase(it);
			break;
		}
	}
}

void FwEngine::SystemObservable::ProcessMessage(IMessage& msg)
{
	std::pair<std::multimap<std::string, SystemListener&>::iterator, 
		std::multimap<std::string, SystemListener&>::iterator> range 
		= _observerMap.equal_range(msg.GetID());
	for (auto it = range.first; it != range.second; ++it)
	{
		it->second.GetHandler(msg.GetID())(msg);
	}
}

FwEngine::SystemObservable::~SystemObservable()
{
	// If Observable is destroyed before listeners, removes itself from listeners
	for (ObserverMap::iterator i = std::begin(_observerMap); i != std::end(_observerMap); ++i)
	{
		i->second._allRegisteredObs.erase(this);
	}
}
