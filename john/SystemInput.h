/******************************************************************************/
/*!
\file		SystemInput.h
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

#ifndef SI_SYSTEM_INPUT
#define SI_SYSTEM_INPUT

#include <Windows.h>
#include <array>
#include "ISystem.h"
#include "SystemObservable.h"

namespace FwEngine
{
	/*=========== Key Codes for all buttons. Still can use regular Virtual Codes, Just not registered here. =============*/
	enum VirtualKey : int
	{
		// Mouse Input
		IMOUSE_LBUTTON = 0x01,
		IMOUSE_RBUTTON = 0x02,
		IMOUSE_MBUTTON = 0x04,

		// Keyboard Input
		// Functions
		IKEY_BACK = 0x08,
		IKEY_TAB = 0x09,
		IKEY_CLEAR = 0x0C,
		IKEY_RETURN = 0x0D,
		IKEY_SHIFT = 0x10,
		IKEY_CTRL = 0x11,
		IKEY_ALT = 0x12,

		IKEY_PAUSE = 0x13,
		IKEY_CAPS = 0x14,

		IKEY_ESCAPE = 0x1B,
		IKEY_SPACE = 0x20,

		IKEY_PGUP = 0x21,
		IKEY_PGDWN = 0x22,
		IKEY_END = 0x23,
		IKEY_HOME = 0x24,

		IKEY_LEFT = 0x25,
		IKEY_UP = 0x26,
		IKEY_RIGHT = 0x27,
		IKEY_DOWN = 0x28,

		IKEY_PRINT = 0x2A,
		IKEY_PRNTSCRN = 0x2C,
		IKEY_INSERT = 0x2D,
		IKEY_DELETE = 0x2E,

		// Numbers
		IKEY_0 = 0x30,
		IKEY_1,
		IKEY_2,
		IKEY_3,
		IKEY_4,
		IKEY_5,
		IKEY_6,
		IKEY_7,
		IKEY_8,
		IKEY_9,


		// Characters
		IKEY_A = 0x41,
		IKEY_B,
		IKEY_C,
		IKEY_D,
		IKEY_E,
		IKEY_F,
		IKEY_G,
		IKEY_H,
		IKEY_I,
		IKEY_J,
		IKEY_K,
		IKEY_L,
		IKEY_M,
		IKEY_N,
		IKEY_O,
		IKEY_P,
		IKEY_Q,
		IKEY_R,
		IKEY_S,
		IKEY_T,
		IKEY_U,
		IKEY_V,
		IKEY_W,
		IKEY_X,
		IKEY_Y,
		IKEY_Z,

		IKEY_NUMPAD_0 = 0x60,
		IKEY_NUMPAD_1,
		IKEY_NUMPAD_2,
		IKEY_NUMPAD_3,
		IKEY_NUMPAD_4,
		IKEY_NUMPAD_5,
		IKEY_NUMPAD_6,
		IKEY_NUMPAD_7,
		IKEY_NUMPAD_8,
		IKEY_NUMPAD_9,

		IKEY_F1 = 0x70,
		IKEY_F2,
		IKEY_F3,
		IKEY_F4,
		IKEY_F5,
		IKEY_F6,
		IKEY_F7,
		IKEY_F8,
		IKEY_F9,
		IKEY_F10,
		IKEY_F11,
		IKEY_F12,

		IKEY_NUMLOCK = 0x90,
		IKEY_SCROLLOCK,
		
		IKEY_LSHIFT = 0xA0,
		IKEY_RSHIFT,
		IKEY_LCTRL,
		IKEY_RCTRL,

		IKEY_FINAL_VALUE = 256 // DO NOT USE: Determines limit of virtual codes used.
	};

	struct CursorPos
	{
		float x;
		float y;
	};

	/*=========================== Internal Input Programming. For developer. =============================*/
	namespace
	{
		class SysBase
		{
		protected:
			HWND _handle;
			bool _debug;
		public:
			SysBase(HWND handle, bool setDebug = false);
			SysBase(const SysBase& rhs);
			~SysBase() = default;
		};

		class SysMousePos : virtual public SysBase
		{
			POINT _mousePos;
			POINT _prevMousePos;
			CursorPos _prevMouseWorldPos;
			bool _cursor;
		protected:
			CursorPos _lastMouseWorldPos;
		public:
			SysMousePos(HWND handle, bool setDebug = false);
			~SysMousePos() = default;

			void Update();

			POINT operator()();

			POINT GetMousePos();
			POINT GetMouseDragVec();
			CursorPos GetMouseWorldPos(float cameraPosX, float cameraPosY, LONG winHeight, LONG winWidth, float zoom);
			CursorPos GetMouseWorldPosDragVec();
			bool ToggleCursor();
			void SetCursor(bool show);
		};

		class SysKeyPress : virtual public SysBase
		{
			enum
			{
				SIReleaseF = 0x01,
				SIPressF = 0x02,

				SIFrameRelease = 0x04,
				SIFramePress = 0x08,
			};
			std::array<unsigned int, IKEY_FINAL_VALUE> _keyHoldState;

			bool OnHold(int key);
			bool OnPress(int key);
			bool OnRelease(int key);

		public:
			enum : int
			{
				HOLD = 0x0,
				PRESS,
				RELEASE,
				PRESSHOLD
			};
			SysKeyPress(HWND handle, bool setDebug = false);
			~SysKeyPress() = default;

			void Update();

			bool operator()(int key, size_t runType = HOLD);
		};
	}

	/*========== Default System Input class. Requires a handle. ==========*/
	class SystemInput : public SysMousePos, public SysKeyPress, public ISystem,
		virtual public SystemObservable
	{
	public:
		SystemInput();
		SystemInput(HWND handle, bool setDebug = false);
		~SystemInput();

		void Init() override;
		void Update(float dt) override;
		void Free() override {}
		void Destroy() override {}

		bool operator()(int key, size_t runtype = HOLD);

		POINT GetMousePos();
		POINT GetMouseDragVec();
		bool ToggleCursor();
		void SetCursor(bool show);
		CursorPos GetMouseWorldPos(float cameraPosX, float cameraPosY, LONG winHeight, LONG winWidth, float zoom = 1.0f);
		CursorPos GetMouseWorldPosDragVec();

		bool GetKeyIf(int key, size_t runtype = HOLD);

		void SetHandle(HWND handle);
	};
}

#endif
