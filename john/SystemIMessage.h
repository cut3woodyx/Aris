/******************************************************************************/
/*!
\file		SystemIMessage.h
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

#ifndef _SYSTEM_IMESSAGE
#define _SYSTEM_IMESSAGE

#include <string>

namespace FwEngine
{
	class IMessage
	{
	protected:
		std::string _id;
	public:
		IMessage() : _id{ "\n" } {}
		IMessage(std::string id) : _id{ id } {}

		std::string GetID() { return _id; }

		virtual ~IMessage() = default;
	};
}

#endif
