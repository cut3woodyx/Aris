/******************************************************************************/
/*!
\file		SystemSerializationBase.h
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

#ifndef _SYSTEM_SERIALIZATION_BASE
#define _SYSTEM_SERIALIZATION_BASE

#include <string>

namespace FwEngine
{
	class ISerializer
	{
	public:
		virtual bool OpenRead(const std::string& file) = 0;

		virtual bool IsGood() = 0;

		virtual void ReadInt(int& i) = 0;
		virtual void ReadFloat(float& f) = 0;
		virtual void ReadString(std::string& str) = 0;
	};

	class ISerializerWrite
	{
	public:
		virtual bool OpenWrite(const std::string& file) = 0;
		virtual bool OpenAppend(const std::string& file) = 0;

		virtual bool IsGood() = 0;

		virtual void Write(const std::string& i) = 0;
	};

	class ISerializableLoad
	{
	public:
		virtual void Load(ISerializer& stream) = 0;
		//virtual ISerializableLoad& operator << (ISerializer& stream);
	};

	class ISerializableSave
	{
	public:
		virtual void Save(ISerializerWrite& stream) = 0;
		//virtual ISerializableSave& operator >> (ISerializer& stream);
	};
}
#endif
