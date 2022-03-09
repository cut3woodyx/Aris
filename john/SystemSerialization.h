/******************************************************************************/
/*!
\file		SystemSerialization.h
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

#ifndef _SYSTEM_SERIALIZATION
#define _SYSTEM_SERIALIZATION

#include "framework.h"
#include "SystemSerializationBase.h"
#include <fstream>
#include <sstream>

namespace FwEngine
{
	/*=============================================================*/
	/*=============== Serializer ====================*/
	/*=============================================================*/
	///Serializer for a simple text file format.
	class TextSerializer : public ISerializer, public ISerializerWrite
	{
	protected:
		std::fstream _stream;
		std::stringstream _buffer;
	public:
		TextSerializer();
		TextSerializer(const std::string& file);

		TextSerializer(const TextSerializer& rhs) = delete;
		TextSerializer(TextSerializer&& rhs) = delete;
		TextSerializer& operator=(const TextSerializer& rhs) = delete;
		TextSerializer& operator=(TextSerializer&& rhs) = delete;

		~TextSerializer();

		virtual bool OpenRead(const std::string& file);
		// Appends to back of file - Can open early
		virtual bool OpenAppend(const std::string& file);
		// WARNING - FILE WILL BE CLEARED
		// Writes Buffer into File and immediately closes file
		// WILL CLEAR CONTENTS IF BUFFER IS EMPTY
		virtual bool OpenWrite(const std::string& file);

		virtual bool IsGood();

		// Run only if Reading File - Does nothing otherwise
		virtual void ReadInt(int& i);
		virtual void ReadFloat(float& f);
		virtual void ReadString(std::string& str);

		virtual void ReadStringLine(std::string& str);

		// Writes to internal Buffer - Does not require opening a file
		virtual void Write(const std::string& i);
		// Pushes buffer into file
		virtual void CloseFile();
		// Clears Buffer
		virtual void Clear();
	};

	/*=============================================================*/
	/*=============== Stream Read ====================*/
	/*=============================================================*/
	// Default Read to an object with a Load
	inline void StreamRead(ISerializer& stream, ISerializableLoad& typeInstance)
	{
		CDEBUG_ASSERT(stream.IsGood(), "File does not exist.");
		typeInstance.Load(stream);
	}
	//Base non recursive stream operators of fundamental types.
	inline void StreamRead(ISerializer& stream, float& f)
	{
		CDEBUG_ASSERT(stream.IsGood(), "File does not exist.");
		stream.ReadFloat(f);
	}
	inline void StreamRead(ISerializer& stream, int& i)
	{
		CDEBUG_ASSERT(stream.IsGood(), "File does not exist.");
		stream.ReadInt(i);
	}
	inline void StreamRead(ISerializer& stream, std::string& str)
	{
		CDEBUG_ASSERT(stream.IsGood(), "File does not exist.");
		stream.ReadString(str);
	}
	/*=============================================================*/
	/*=============== Stream Write ====================*/
	/*=============================================================*/

	// Default Write to an object with a Save
	inline void StreamWrite(ISerializerWrite& stream, ISerializableSave& typeInstance)
	{
		typeInstance.Save(stream);
	}
	inline void StreamWrite(ISerializerWrite& stream, std::string& str)
	{
		stream.Write(str);
	}
	inline void StreamWriteToFile(ISerializerWrite& stream, const std::string& filename)
	{
		stream.OpenWrite(filename);
	}
}

#endif
