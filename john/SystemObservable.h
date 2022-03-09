/******************************************************************************/
/*!
\file		SystemObservable.h
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

#ifndef _SYSTEM_OBSERVABLE
#define _SYSTEM_OBSERVABLE

#include "SystemListener.h"

namespace FwEngine
{
	class SystemObservable
	{
		typedef std::multimap<std::string, SystemListener&> ObserverMap;

		std::string _id;
		ObserverMap _observerMap;
	public:
		SystemObservable();
		SystemObservable(std::string id);

		void Register(const std::string msg_ID, SystemListener& listener);
		void Unregister(const std::string msg_ID, SystemListener& listener);

		void ProcessMessage(IMessage& msg);
		
		~SystemObservable();
	};
}


#endif