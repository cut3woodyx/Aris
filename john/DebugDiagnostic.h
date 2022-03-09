/******************************************************************************/
/*!
\file		DebugDiagnostic.h
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
#pragma once

#include <assert.h>
#include "DebugToStream.h"

//Define the debug break using the MS specific or general using assembly interrupt 3
#ifdef _MSC_VER
#define G_DEBUG_BREAK __debugbreak()
#else
#define G_DEBUG_BREAK do { __asm int 3 } while(0)
#endif

//By defining G_ENABLE_DEBUG_DIAGNOSTICS you can explicitly 
//enable or disable debugging and diagnostic macros
#if !defined(G_ENABLE_DEBUG_DIAGNOSTICS) 
#   if defined(NDEBUG)
#       define G_ENABLE_DEBUG_DIAGNOSTICS 0
#   else
#       define G_ENABLE_DEBUG_DIAGNOSTICS 1
#   endif
#endif

//Implement these functions to control how errors and
//debug printing are handled
bool SignalErrorHandler(const char* expression, const char* file, int line, const char* formatMessage = 0);

bool ReleaseErrorHandler(const char* expression, const char* file, int line, const char* formatMessage = 0);

#if G_ENABLE_DEBUG_DIAGNOSTICS

//If diagnostics are enabled use the debug functions
#define CDEBUG_PRINT(...)  Debug_ToStream(std::cout),__VA_ARGS__; //DebugPrintHandler( __VA_ARGS__ );

// The do/while blocks embedded here are ABSOLUTELY NECESSARY to prevent
// bizzare compilation errors. 
// Check out http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
// for more details.

#define CDEBUG_ERROR_IF(exp, ...) \
	do { if( (exp) && SignalErrorHandler(#exp, __FILE__, __LINE__,__VA_ARGS__ )) \
	throw std::exception{}; } while(0)
	//assert((!(exp)));} while (0)
	//G_DEBUG_BREAK; } while(0)

#define CDEBUG_ASSERT(exp, ...) \
	do { if( (!(exp)) && SignalErrorHandler(#exp, __FILE__, __LINE__,__VA_ARGS__ )) \
	throw std::exception{}; } while(0)
	//assert(((exp)));} while (0)
	//G_DEBUG_BREAK; } while(0)

#else

//If the diagnostics are disabled all the debug functions
//are no ops

#define CDEBUG_PRINT(...) ((void)0)

#define CDEBUG_ERROR_IF(exp, ...) \
	do { if( (exp) && ReleaseErrorHandler(#exp, __FILE__, __LINE__,__VA_ARGS__ )) \
	throw std::exception{}; } while(0)

#define CDEBUG_ASSERT(exp, ...) \
	do { if( (!(exp)) && ReleaseErrorHandler(#exp, __FILE__, __LINE__,__VA_ARGS__ )) \
	throw std::exception{}; } while(0)

#endif
