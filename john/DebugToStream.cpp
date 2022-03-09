/******************************************************************************/
/*!
\file		DebugToStream.cpp
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

#include "DebugToStream.h"
#include <fstream>

Debug_ToStream::Debug_ToStream(std::ostream& s, const std::string& separator)
	: _stream(s), _hasEntries(false), _separator(separator) {}

Debug_ToStream::~Debug_ToStream()
{
	_stream << std::endl;
}

// Open and write to file
void Debug_LogToFile(const std::string& filename, const std::string& msg)
{
	std::ofstream file;
	file.open(filename, std::ios_base::app);
	Debug_ToStream debugStrm(file);
	debugStrm(msg);
}
