/******************************************************************************/
/*!
\file		DebugToStream.h
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

#ifndef _DEBUG_TOSTREAM
#define _DEBUG_TOSTREAM

#include <iostream>
#include <fstream>

class Debug_ToStream
{
	std::ostream& _stream;
	bool _hasEntries;
	std::string _separator;

	template <typename T>
	Debug_ToStream& ToStream(const T& v)
	{
		if (_hasEntries)
			_stream << _separator;
		_stream << v;
		_hasEntries = true;

		return *this;
	}
public:
	Debug_ToStream(std::ostream& s, const std::string& separator = " ");

	template<typename T>
	Debug_ToStream& operator , (const T& v)
	{
		ToStream(v);
		return *this;
	}

	template<typename T, typename... TArgs>
	Debug_ToStream& operator()(const T& v, TArgs... args)
	{
		ToStream(v);
		operator()(args...);
		return *this;
	}
	template<typename T>
	Debug_ToStream& operator()(const T& v)
	{
		ToStream(v);
		_stream << std::endl;
		return *this;
	}
	~Debug_ToStream();
};

void Debug_LogToFile(const std::string& filename, const std::string& msg);

#endif;
