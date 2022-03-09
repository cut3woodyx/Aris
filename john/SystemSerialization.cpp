/******************************************************************************/
/*!
\file		SystemSerialization.cpp
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

#include "SystemSerialization.h"

namespace FwEngine
{
	TextSerializer::TextSerializer() : _buffer{} {}
	TextSerializer::TextSerializer(const std::string& file) : _buffer{}
	{
		OpenRead(file);
	}

	TextSerializer::~TextSerializer()
	{
		CloseFile();
	}

	// Open file for reading
	bool TextSerializer::OpenRead(const std::string& file)
	{
		CloseFile();
		_stream.open(file.c_str(), std::ios_base::in);
		return _stream.is_open();
	}

	// Open file to write and close
	bool TextSerializer::OpenWrite(const std::string& file)
	{
		_stream.open(file.c_str(), std::ios_base::out | std::ios_base::trunc);
		bool ret = _stream.is_open();
		CDEBUG_ASSERT(ret, std::string("Unable to Save to File: ").append(file).c_str());
		CloseFile();
		return ret;
	}

	// Open file to append
	bool TextSerializer::OpenAppend(const std::string& file)
	{
		CloseFile();
		_stream.open(file.c_str(), std::ios_base::out | std::ios_base::app);
		return _stream.is_open();
	}

	bool TextSerializer::IsGood()
	{
		return _stream.good() && !_stream.eof();
	}

	void TextSerializer::ReadInt(int& i)
	{
		std::string line;
		ReadString(line);
		i = std::stoi(line);
		//_stream >> i;
	}

	void TextSerializer::ReadFloat(float& f)
	{
		std::string line;
		ReadString(line);
		f = std::stof(line);
		//_stream >> f;
	}

	void TextSerializer::ReadString(std::string& str)
	{
		_stream >> str;
	}

	void TextSerializer::ReadStringLine(std::string& str)
	{
		std::getline(_stream, str);
	}

	void TextSerializer::Write(const std::string& str)
	{
		_buffer << "\n" << str;
		//_stream.flush();
	}

	void TextSerializer::Clear()
	{
		_buffer.str(std::string());
	}

	void TextSerializer::CloseFile()
	{
		_buffer.ignore();
		_stream << _buffer.rdbuf();
		_stream.close();
		_stream.clear();
		Clear();
	}

}