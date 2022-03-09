/******************************************************************************/
/*!
\file		WindowSystem.cpp
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "WindowSystem.h"
#include "Resource.h"
#include "SystemGraphics.h"
#include "SystemAudio.h"
#include "SystemInput.h"
#include "SystemSerialization.h"
#include "StringConverter.h"
#include "FwAudio.h"

#ifdef _LEVEL_EDITOR	// LEVEL EDITOR ONLY
#include "imgui.h"
#endif

namespace FwEngine
{
	WindowSystem* WINDOWMAIN = nullptr;

	//Handles messages for winAPI
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			(void)hdc;
			//Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
		{
			if (GRAPHICS)
			{
				RECT newRect;
				GetClientRect(hWnd, &newRect);

				int width = newRect.right - newRect.left;
				int height = newRect.bottom - newRect.top;

				WINDOWMAIN->SetWindowSize(width, height);

				GRAPHICS->UpdateProjection((float)width, (float)height);
			}
		}
		break;

#ifdef _LEVEL_EDITOR	// LEVEL EDITOR ONLY
		case WM_MOUSEWHEEL:
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;

			//RECT newRect;
			//GetClientRect(hWnd, &newRect);

			/*
			RECT winRec = { 0,0,width, height };
			AdjustWindowRect(&winRec, WS_OVERLAPPEDWINDOW, FALSE);
			*/
			if (!io.WantCaptureMouse)
			{
				GRAPHICS->_zoom += -io.MouseWheel * 0.1f;
				if (GRAPHICS->_zoom < 0.1f)
					GRAPHICS->_zoom = 0.1f;
				else if (GRAPHICS->_zoom > 10.0f)
					GRAPHICS->_zoom = 10.0f;

				GRAPHICS->UpdateZoom();
				//GRAPHICS->UpdateProjection((float)newRect.right, (float)newRect.bottom);
			}
			break;
		}
		
		case WM_CHAR:
		{
			ImGuiIO& io = ImGui::GetIO();
			io.AddInputCharacter((unsigned int)wParam);
		}
		
#endif
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	WindowSystem::WindowSystem() try : _handle{}, _instance{}, _windowsTitle{}, _windowClassName{}, _cursorFileName{},
		 _currentWidth{ 0 }, _currentHeight{ 0 }, _input{ nullptr }, _resolutions{}, _fullScreenMode{ false },
		_resolutionWidth{ 0 }, _resolutionHeight{ 0 }, _currentResolutionIndex{}, _fullScreen{ false }, _masterVolume{ 1.0f }
	{
		WINDOWMAIN = this;
		LoadWindow(FILE_PATH);
		//ParamValueMap temp;
		//InitWindow(temp);
	}
	catch (...)
	{
		throw;
	}

	WindowSystem::~WindowSystem()
	{
		UnregisterClass(_windowClassName.c_str(),_instance);
		//SaveWindow();
	}

	HWND WindowSystem::GetWinHandle()
	{
		return _handle;
	}

	HINSTANCE WindowSystem::GetWinInstance()
	{
		return _instance;
	}

	int WindowSystem::GetWinWidth()
	{
		return _currentWidth;
	}

	int WindowSystem::GetWinHeight()
	{
		return _currentHeight;
	}

	void WindowSystem::SetWindowSize(int width, int height)
	{
		_currentWidth = width;
		_currentHeight = height;
	}

	void WindowSystem::SetMasterVolume()
	{
		::FwEngine::SetMasterVolume(AUDIO->GetSystem(), _masterVolume);
	}

	std::vector<std::string>& WindowSystem::GetResolutions()
	{
		return _resolutions;
	}

	void WindowSystem::SetResolutionFromString(std::string& resolution)
	{
		std::string width;
		std::string height;

		width = resolution.substr(0, resolution.find("x"));
		height = resolution.substr(resolution.find("x") + 1);

		_resolutionWidth = std::stoi(width);
		_resolutionHeight = std::stoi(height);
	}

	void WindowSystem::ChangeResolution()
	{
		SetResolutionFromString(_resolutions[_currentResolutionIndex]);

		if (GRAPHICS)
		{
			GRAPHICS->ChangeResolution(_resolutionWidth, _resolutionHeight);
			GRAPHICS->UpdateProjection((float)_currentWidth, (float)_currentHeight);
		}
	}

	SystemInput* WindowSystem::GetInputSystem()
	{
		return _input;
	}

	void WindowSystem::SetInputSystem(SystemInput* input)
	{
		_input = input;
	}

	std::unordered_map<std::string, ParamValueMap> WindowSystem::GetWindowConfiguration()
	{
		ParamValueMap resolutions;
		ParamValueMap current;
		std::unordered_map<std::string, ParamValueMap> returnMap;

		int i = 0;
		for (auto it : _resolutions)
		{
			resolutions.emplace(std::string(OPTIONS).append(std::to_string(i)), it);
			++i;
		}

		returnMap.emplace(RESOLUTIONS, resolutions);

		current.emplace(CURRENT_RESOLUTION, std::to_string(_currentResolutionIndex));
		current.emplace(FULLSCREEN, std::to_string(_fullScreen));

		//Conversion from wstring to string
		/*size_t size = 0;
		_locale_t lc = _create_locale(LC_ALL, "en_SG.UTF-8");
		errno_t err = _wcstombs_s_l(&size, NULL, 0, &_windowsTitle[0], _TRUNCATE, lc);
		std::string title = std::string(size, 0);
		err = _wcstombs_s_l(&size, &title[0], size, &_windowsTitle[0], _TRUNCATE, lc);
		_free_locale(lc);
		title.resize(size - 1);*/

		//UTF8_encode(_windowsTitle)
		current.emplace(GAME_TITLE, UTF8_encode(_windowsTitle));
		current.emplace(MASTER_VOLUME, std::to_string(_masterVolume));

		returnMap.emplace(WINDOW, current);

		return returnMap;
	}

	void WindowSystem::LoadWindow(std::string fileName)
	{
		CustomTextSerializer readFromFile;
		CDEBUG_ASSERT(readFromFile.OpenRead(fileName), "Unable to Open File!");

		std::string configurations;
		std::unordered_map<std::string, ParamValueMap> configMap;

		//Taken from system manager to reuse.
		readFromFile.CustomReadObjComponent(configurations, configMap);
		CDEBUG_ERROR_IF(configurations != CONFIG, "Incorrect initialization");
		InitWindow(configMap);
	}

	void WindowSystem::SaveWindow()
	{
		typedef std::pair<std::string, std::unordered_map<std::string, ParamValueMap>> vecPair;
		std::vector<vecPair> fullObjMap;
		CustomTextSerializer saveWindow;

		std::unordered_map<std::string, ParamValueMap> temp = WINDOWMAIN->GetWindowConfiguration();
		fullObjMap.push_back(vecPair(CONFIG, temp));
		saveWindow.CustomSaveObjComponent(FILE_PATH, fullObjMap);
	}

	void WindowSystem::DisableClose()
	{
		SetClassLong(_handle, GCL_STYLE,
			GetClassLong(_handle, GCL_STYLE) | CS_NOCLOSE);
	}

	void WindowSystem::EnableClose()
	{
		SetClassLong(_handle, GCL_STYLE,
			GetClassLong(_handle, GCL_STYLE) & ~CS_NOCLOSE);
	}

	void WindowSystem::InitWindow(std::unordered_map<std::string, ParamValueMap> initMap)
	{
		int i = 0;
		auto it = initMap.find(RESOLUTIONS);
		auto end = initMap.end();

		//Get the options of resolutions
		while (it != end)
		{
			std::string resolutions;
	
			if (it->second.find(std::string(OPTIONS).append(std::to_string(i))) != it->second.end())
			{
				resolutions = it->second.find(std::string(OPTIONS).append(std::to_string(i)))->second;
				_resolutions.push_back(resolutions);
			}
			else
			{
				break;
			}

			++i;
		}

		it = initMap.find(WINDOW);

		if (it != end)
		{
			if (it->second.find(CURRENT_RESOLUTION) != it->second.end())
			{
				_currentResolutionIndex = std::stoi(it->second.find(CURRENT_RESOLUTION)->second);
				CDEBUG_ERROR_IF(_currentResolutionIndex > _resolutions.size(), "Index too big for resolution");
				SetResolutionFromString(_resolutions[_currentResolutionIndex]);
			}
			else
			{
				throw;
			}

			if (it->second.find(FULLSCREEN) != it->second.end())
			{
				_fullScreen = std::stoi(it->second.find(FULLSCREEN)->second);
				//_fullScreenMode = _fullScreen;
			}

			if (it->second.find(GAME_TITLE) != it->second.end())
			{
				_windowsTitle = UTF8_decode(it->second.find(GAME_TITLE)->second);
			}

			if (it->second.find(MASTER_VOLUME) != it->second.end())
			{
				_masterVolume = std::stof(it->second.find(MASTER_VOLUME)->second);
			}
		}

		_windowClassName = L"FrameworkEngineWindowClass";
#ifdef _LEVEL_EDITOR
		//Define the window class for the game.
		WNDCLASSEX wc = { sizeof(WNDCLASSEX),	//The size of this structure (passing the size allows Microsoft to update their interfaces and maintain backward compatibility)
						  CS_CLASSDC,							//The style of the window class--this is the base type (one device context for all windows in the process)
						  MessageHandler,						//The name of the message handling function
						  0L, 0L,								//The amount of extra memory to allocate for this class and window
						  GetModuleHandle(NULL),				//Handle to the instance that has the windows procedure--NULL means use this file.
						  NULL,								//Add an Icon as a resource and add them here
						  LoadCursor(NULL, IDC_ARROW),		//Use the default arrow cursor
						  NULL, NULL,							//The background brush and menu--these can be NULL
						  _windowClassName.c_str(), NULL };			//The class name and the small icon (NULL just uses the default)

		//Register the class
		RegisterClassEx(&wc);

		//Store the handle to the instance
		_instance = wc.hInstance;

		RECT winRec = { 0,0,_resolutionWidth, _resolutionHeight };
		AdjustWindowRect(&winRec, WS_OVERLAPPEDWINDOW, FALSE);
		//Create the game's window

		_handle = CreateWindow(_windowClassName.c_str(),	//The class name
							   _windowsTitle.c_str(),		//The name for the title bar
							   (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW),//The style of the window (WS_BORDER, WS_MINIMIZEBOX, WS_MAXIMIZE, etc.)
							   0, 0,		//The x and y position of the window (screen coords for base windows, relative coords for child windows)
							   winRec.right - winRec.left,	//Width of the window, including borders
							   winRec.bottom - winRec.top,	//Height of the window, including borders and caption
							   GetDesktopWindow(),					//The parent window
							   NULL,								//The menu for the window
							   _instance,							//The handle to the instance of the window (ignored in NT/2K/XP)
							   NULL);								//The lParam for the WM_CREATE message of this window
#else
		_cursorFileName = L"Resources\\UI\\ui_mousecursor.cur";
		HCURSOR cursor = LoadCursorFromFile(_cursorFileName.c_str());

		//Define the window class for the game.
		WNDCLASSEX wc = { sizeof(WNDCLASSEX),	//The size of this structure (passing the size allows Microsoft to update their interfaces and maintain backward compatibility)
						  CS_CLASSDC,							//The style of the window class--this is the base type (one device context for all windows in the process)
						  MessageHandler,						//The name of the message handling function
						  0L, 0L,								//The amount of extra memory to allocate for this class and window
						  GetModuleHandle(NULL),				//Handle to the instance that has the windows procedure--NULL means use this file.
						  NULL,								//Add an Icon as a resource and add them here
						  cursor,							//Use the default arrow cursor
						  NULL, NULL,							//The background brush and menu--these can be NULL
						  _windowClassName.c_str(), NULL };			//The class name and the small icon (NULL just uses the default)

		//Register the class
		RegisterClassEx(&wc);
		//SetCursor(cursor);

		//Store the handle to the instance
		_instance = wc.hInstance;

		RECT winRec = { 0,0,_resolutionWidth, _resolutionHeight };
		AdjustWindowRect(&winRec, WS_OVERLAPPEDWINDOW, FALSE);
		//Create the game's window
		//Don't allow resizing window size in game except through options which has fixed resolutions
		_handle = CreateWindow(_windowClassName.c_str(),	//The class name
								_windowsTitle.c_str(),		//The name for the title bar
								(WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX,//The style of the window (WS_BORDER, WS_MINIMIZEBOX, WS_MAXIMIZE, etc.)
								0, 0,		//The x and y position of the window (screen coords for base windows, relative coords for child windows)
								winRec.right - winRec.left,	//Width of the window, including borders
								winRec.bottom - winRec.top,	//Height of the window, including borders and caption
								GetDesktopWindow(),					//The parent window
								NULL,								//The menu for the window
								_instance,							//The handle to the instance of the window (ignored in NT/2K/XP)
								NULL);
#endif

		if (_fullScreen)
		{
			ToggleFullScreen(false);
		}
	}

	WINMAIN_MESSAGE WindowSystem::Update(float dt)
	{
		(void)dt;
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				return WINMAIN_QUIT;
		}

		return WINMAIN_NOMESSAGE;
	}

	void WindowSystem::ActivateWindow()
	{
#ifdef _LEVEL_EDITOR
		ShowWindow(_handle, SW_SHOWMAXIMIZED);
#else
		ShowWindow(_handle, SW_SHOWDEFAULT);
#endif 
		RECT rect;
		GetClientRect(_handle, &rect);
		_currentWidth = rect.right - rect.left;
		_currentHeight = rect.bottom - rect.top;
	}

	void WindowSystem::ToggleFullScreen(bool flip)
	{
		if (flip)
		{
			_fullScreen = !_fullScreen;
		}

		if (_fullScreen == _fullScreenMode)
		{
			return;
		}

		if (_fullScreen)
		{
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);
			SetWindowLongPtr(_handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(_handle, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);

			if (GRAPHICS)
			{
				GRAPHICS->UpdateProjection((float)width, (float)height);
			}
			//This means graphics has not been created.
			else
			{
				_currentWidth = width;
				_currentHeight = height;
			}
			_fullScreenMode = _fullScreen;
		}
		else
		{
			RECT winRec = { 0,0,_currentWidth, _currentHeight };
			AdjustWindowRect(&winRec, WS_OVERLAPPEDWINDOW, FALSE);

			SetWindowLongPtr(_handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_MAXIMIZE);
			SetWindowPos(_handle, NULL, 0, 0, winRec.right - winRec.left, winRec.bottom - winRec.top, SWP_FRAMECHANGED);
			GRAPHICS->UpdateProjection((float)_currentWidth, (float)_currentHeight);

			_fullScreenMode = _fullScreen;
		}
	}
	bool WindowSystem::GetFullScreenState()
	{
		return _fullScreenMode;
	}
}