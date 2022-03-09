/******************************************************************************/
/*!
\file		SystemListener.cpp
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

#include "framework.h"
#include "SystemListener.h"
#include "SystemObservable.h"
#include <algorithm>    // std::for_each

namespace FwEngine
{
	SystemListener::SystemListener() : _id { "\n" }
	{
	}
	SystemListener::SystemListener(std::string id) : _id { id }
	{
	}

	void SystemListener::RemoveHandler(const std::string& messageID)
	{
		_handleMap.erase(messageID);
	}

	MESSAGE_HANDLER SystemListener::GetHandler(const std::string& messageId)
	{
		CDEBUG_ERROR_IF(_handleMap.find(messageId) == _handleMap.end(), 
			"Handler does not exist!");
		return _handleMap[messageId];
	}

	// Removes itself automatically from all Observables
	SystemListener::~SystemListener()
	{
		std::for_each(std::begin(_allRegisteredObs), std::end(_allRegisteredObs), 
			[=](SystemObservable* obs) 
			{
				for (decltype(_handleMap)::iterator i = std::begin(_handleMap); i != std::end(_handleMap); ++i)
				{
					obs->Unregister(i->first, *this);
				}
			});
	}
}
