/******************************************************************************/
/*!
\file		DebugDiagnostic.cpp
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

#include "DebugDiagnostic.h"
#include <sstream>
#include <exception>
#include "WindowSystem.h"

//A basic error output function
bool SignalErrorHandler(const char* exp, const char* file, int line, const char* msg)
{
	Debug_ToStream stream(std::cerr);
	stream("Debug Expression: ", exp);
	stream("\nFilename: ", file);
	stream("\nLine: ", line);
	if (msg)
		stream("\nMessage: ", msg);

	std::stringstream log;
	//log << "Date of Error: " << __DATE__;
	//log << "\nTime of Error: " << __TIME__;
	log << "Debug Expression: " << exp;
	log << "\nFilename: " << file;
	log << "\nLine: " << line;
	if (msg)
		log << "\nMessage: " << msg;

	std::wstring wstr(log.str().length(), L' '); // Make room for characters
	std::string str(log.str());

	// Copy string to wstring.
	std::copy(std::begin(str), std::end(str), wstr.begin());

	if (MessageBox(FwEngine::WINDOWMAIN->GetWinHandle(), wstr.c_str(), NULL, MB_ABORTRETRYIGNORE | MB_ICONERROR)
		== IDABORT)
		abort();

	return true;
}

bool ReleaseErrorHandler(const char* exp, const char* file, int line, const char* msg)
{
	// Log details into stream
	std::stringstream log;
	//log << "Date of Error: " << __DATE__;
	//log << "\nTime of Error: " << __TIME__;
	log << "Debug Expression: " << exp;
	log << "\nFilename: " << file;
	log << "\nLine: " << line;
	if (msg)
		log << "\nMessage: " << msg;

	// Create and write to Log
	std::fstream obj(".\\Resources\\Logs");
	if (!obj.is_open())
		CreateDirectory(L".\\Resources\\Logs", NULL);
	Debug_LogToFile("Resources/Logs/Error_Log.txt", log.str());

	std::wstring wstr(log.str().length(), L' '); // Make room for characters
	std::string str(log.str());

	// Copy string to wstring.
	std::copy(std::begin(str), std::end(str), wstr.begin());

	if (MessageBox(FwEngine::WINDOWMAIN->GetWinHandle(), wstr.c_str(), NULL, MB_ABORTRETRYIGNORE | MB_ICONERROR)
		== IDABORT)
		abort();

	return true;
}
