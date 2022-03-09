/******************************************************************************/
/*!
\file		SystemInput.cpp
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
#include "SystemInput.h"
#include <string>
#include "SystemCustomMessages.h"

namespace FwEngine
{
	/***************************************************************************************************/
	/*================================================================*/
	/*=====	SysBase ======*/
	/*================================================================*/
	SysBase::SysBase(HWND handle, bool d) : _handle{ handle }, _debug{ d }
	{}

	SysBase::SysBase(const SysBase& rhs)
	{
		_handle = rhs._handle;
		_debug = rhs._debug;
	}

	/***************************************************************************************************/
	/*================================================================*/
	/*=====	SysMousePos ======*/
	/*================================================================*/

	SysMousePos::SysMousePos(HWND handle, bool d) : SysBase{ handle, d }, _mousePos{}, _prevMousePos{}, _cursor{ true }
	{
	}

	void SysMousePos::Update()
	{
		_prevMousePos = _mousePos;
		_prevMouseWorldPos = { _lastMouseWorldPos.x, _lastMouseWorldPos.y };
	}

	inline POINT SysMousePos::operator()()
	{
		return GetMousePos();
	}

	POINT SysMousePos::GetMousePos()
	{
		// Change to get current window handle
		if (GetCursorPos(&_mousePos) && ScreenToClient(_handle, &_mousePos))
		{
			//CDEBUG_PRINT("Mouse Position: " + std::to_string(_mousePos.x) + " " + std::to_string(_mousePos.y));
			return _mousePos;
		}

		//CDEBUG_PRINT("Failed to get Cursor Position OR Failed to Convert Mouse Position to Window position.");
		return _mousePos;
	}

	POINT SysMousePos::GetMouseDragVec()
	{
		return { _prevMousePos.x - _mousePos.x,  _prevMousePos.y - _mousePos.y };
	}


	CursorPos SysMousePos::GetMouseWorldPos(float cameraPosX, float cameraPosY, LONG winHeight, LONG winWidth, float zoom = 1.0f)
	{
		RECT rect;
		GetClientRect(_handle, &rect);
		LONG width = rect.right;
		LONG height = rect.bottom;
		POINT pos = GetMousePos();
		//return { ((cameraPosX - (winWidth * 0.5f)) + (float)pos.x / (float)width * winWidth),
		//		(cameraPosY - (winHeight * 0.5f)) + winHeight - (float)pos.y / (float)height * winHeight};

		float x = (cameraPosX - (winWidth * 0.5f)) + (float)pos.x / (float)width * winWidth;
		float y = (cameraPosY + (winHeight * 0.5f)) - (float)pos.y / (float)height * winHeight;

		//float x_diff = cameraPosX - x;
		//float y_diff = cameraPosY - y;
		//return { (x + x_diff) - x_diff * zoom,
		//		 (y + y_diff) - y_diff * zoom };

		_lastMouseWorldPos = { cameraPosX - (cameraPosX - x) * zoom,
							cameraPosY - (cameraPosY - y) * zoom };

		//_lastMouseWorldPos = { (float)pos.x, (float)pos.y };

		return _lastMouseWorldPos;
	}

	CursorPos SysMousePos::GetMouseWorldPosDragVec()
	{
		return { _lastMouseWorldPos.x - _prevMouseWorldPos.x, _lastMouseWorldPos.y - _prevMouseWorldPos.y };
	}

	bool SysMousePos::ToggleCursor()
	{
		CDEBUG_PRINT("Toggle Cursor: " + std::to_string(_cursor));
		_cursor ^= 1;

		if (_cursor)
			while (ShowCursor(true) < 0);
		else
			while (ShowCursor(false) > 0);

		return _cursor;
	}

	void SysMousePos::SetCursor(bool show)
	{
		CDEBUG_PRINT("Set Cursor: " + std::to_string(show));
		_cursor = show;

		if (show)
			while (ShowCursor(true) < 0);
		else
			while (ShowCursor(false) > 0);
	}

	/***************************************************************************************************/
	/*================================================================*/
	/*=====	SysKeyPress ======*/
	/*================================================================*/

	SysKeyPress::SysKeyPress(HWND handle, bool d) : SysBase{ handle, d }, _keyHoldState{ 0 }
	{
		for (unsigned int& i : _keyHoldState)
			i |= SIReleaseF;
	}

	// Only run once in main loop - either at beginning or end of loop
	void SysKeyPress::Update()
	{
		for (unsigned int& i : _keyHoldState)
		{
			i &= ~SIFramePress & ~SIFrameRelease;
		}
	}

	bool SysKeyPress::operator()(int key, size_t runType)
	{
		//CDEBUG_ASSERT(key < IKEY_FINAL_VALUE, "Key Press Code does not exist.");

		if (GetForegroundWindow() != _handle)
		{
			//CDEBUG_PRINT("Window is Out of Focus.");
			return false;
		}

		bool ifTrue = false;

		switch (runType)
		{
		case HOLD:
			ifTrue = OnHold(key);
			if (ifTrue)
				CDEBUG_PRINT("On Hold Key: " + std::to_string(key));
			break;
		case PRESS:
			ifTrue = OnPress(key);
			if (ifTrue)
				CDEBUG_PRINT("On Press Key: " + std::to_string(key));
			break;
		case RELEASE:
			ifTrue = OnRelease(key);
			if (ifTrue)
				CDEBUG_PRINT("On Release Key: " + std::to_string(key));
			break;
		default:
			CDEBUG_PRINT("Key Run Type Not Found: " + std::to_string(runType));
			return false;
		}
		return ifTrue;
	}

	inline bool SysKeyPress::OnHold(int key)
	{
		return GetAsyncKeyState(key) & 0x8000;
	}

	bool SysKeyPress::OnPress(int key)
	{
		if (OnHold(key))
		{
			if (_keyHoldState[key] & SIFramePress)
				return true;

			if (!(_keyHoldState[key] & SIPressF))
			{
				_keyHoldState[key] |= SIPressF | SIFramePress;
				return true;
			}
		}
		else
		{
			_keyHoldState[key] &= ~SIPressF & ~SIFramePress;
		}

		return false;

	}

	bool SysKeyPress::OnRelease(int key)
	{
		if (OnHold(key))
		{
			_keyHoldState[key] &= ~SIReleaseF & ~SIFrameRelease;
		}
		else
		{
			if (_keyHoldState[key] & SIFrameRelease)
				return true;

			if (!(_keyHoldState[key] & SIReleaseF))
			{
				_keyHoldState[key] |= SIReleaseF | SIFrameRelease;
				return true;
			}
		}
		return false;
	}

	/***************************************************************************************************/
	/*================================================================*/
	/*=====	SystemInput =====*/
	/*================================================================*/

	SystemInput::SystemInput() : SystemInput(GetForegroundWindow())
	{
	}

	SystemInput::SystemInput(HWND handle, bool d)
		: SysBase{ handle, d }, SysMousePos{ handle, d }, SysKeyPress{ handle, d }, SystemObservable{"SystemInput"}
	{
	}

	SystemInput::~SystemInput()
	{
	}

	void SystemInput::Init()
	{
	}

	void SystemInput::Update(float /*dt*/)
	{
		//(void)dt;
		SysKeyPress::Update();
		SysMousePos::Update();

		/*
		POINT mousePos = {(LONG)_lastMouseWorldPos.x, (LONG)_lastMouseWorldPos.y};

		// TESTING PURPOSES ONLY - SYSTEM MESSAGING
		if (GetKeyIf(IMOUSE_LBUTTON, PRESS))
		{
			InputMessage msg{ mousePos, IMOUSE_LBUTTON };
			ProcessMessage(msg);
		}
		if (GetKeyIf(IKEY_Q, HOLD))
		{
			InputMessage msg{ mousePos, IKEY_Q };
			ProcessMessage(msg);
		}
		if (GetKeyIf(IMOUSE_RBUTTON, PRESS))
		{
			InputMessage msg{ mousePos, IMOUSE_RBUTTON };
			ProcessMessage(msg);
		}
		if (GetKeyIf(IKEY_TAB, PRESS))
		{
			InputMessage msg{ mousePos, IKEY_TAB };
			ProcessMessage(msg);
		}
		if (GetKeyIf(IKEY_SPACE, PRESS))
		{
			InputMessage msg{ mousePos, IKEY_SPACE };
			ProcessMessage(msg);
		}
		*/
	}

	bool SystemInput::operator()(int key, size_t runtype)
	{
		return SysKeyPress::operator()(key, runtype);
	}

	POINT SystemInput::GetMousePos()
	{
		return SysMousePos::GetMousePos();
	}

	POINT SystemInput::GetMouseDragVec()
	{
		return SysMousePos::GetMouseDragVec();
	}

	bool SystemInput::ToggleCursor()
	{
		return SysMousePos::ToggleCursor();
	}

	void SystemInput::SetCursor(bool show)
	{
		SysMousePos::SetCursor(show);
	}

	CursorPos SystemInput::GetMouseWorldPos(float cameraPosX, float cameraPosY, LONG winHeight, LONG winWidth, float zoom)
	{
		return SysMousePos::GetMouseWorldPos(cameraPosX, cameraPosY, winHeight, winWidth, zoom);
	}

	CursorPos SystemInput::GetMouseWorldPosDragVec()
	{
		return SysMousePos::GetMouseWorldPosDragVec();
	}

	bool SystemInput::GetKeyIf(int key, size_t runtype)
	{
		return SysKeyPress::operator()(key, runtype);
	}

	void SystemInput::SetHandle(HWND handle)
	{
		_handle = handle;
	}

}

