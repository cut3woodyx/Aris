/******************************************************************************/
/*!
\file		WindowSystem.h
\project	Aris
\\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include <stdlib.h>
#include "Framework.h"
#include "SystemInput.h"
#include "SystemCustomSerialization.h"

namespace FwEngine
{
	constexpr auto FILE_PATH = "Resources/Configuration/WindowsConfig.txt";
	constexpr auto CONFIG = "Configuration";
	constexpr auto RESOLUTIONS = "Resolution";
	constexpr auto OPTIONS = "Option";
	constexpr auto WINDOW = "Window";
	constexpr auto CURRENT_RESOLUTION = "CurrentResolution";
	constexpr auto FULLSCREEN = "Fullscreen";
	constexpr auto GAME_TITLE = "Title";
	constexpr auto MASTER_VOLUME = "Volume";

	enum WINMAIN_MESSAGE
	{
		WINMAIN_NOMESSAGE,
		WINMAIN_QUIT,
	};

	class WindowSystem
	{
	private:
		HWND _handle;
		HINSTANCE _instance;
		std::wstring _windowsTitle;
		std::wstring  _windowClassName;
		std::wstring  _cursorFileName;

		int _currentWidth;
		int _currentHeight;

		SystemInput* _input;
		std::vector<std::string> _resolutions;
		
		bool _fullScreenMode;

	public:
		int _resolutionWidth;
		int _resolutionHeight;
		int _currentResolutionIndex;
		bool _fullScreen;
		float _masterVolume;

		WindowSystem();
		~WindowSystem();

		HWND GetWinHandle();
		HINSTANCE GetWinInstance();

		int GetWinWidth();
		int GetWinHeight();
		void SetWindowSize(int width, int height);
		
		void SetMasterVolume();

		std::vector<std::string>& GetResolutions();

		void ChangeResolution();
		void SetResolutionFromString(std::string& resolution);

		SystemInput* GetInputSystem();
		void SetInputSystem(SystemInput* input);

		std::unordered_map<std::string, ParamValueMap> GetWindowConfiguration();
		void LoadWindow(std::string fileName);
		void SaveWindow();

		void DisableClose();
		void EnableClose();

		void InitWindow(std::unordered_map<std::string, ParamValueMap> initMap);
		WINMAIN_MESSAGE Update(float dt);
		void ActivateWindow();
		void ToggleFullScreen(bool flip = true);
		bool GetFullScreenState();
	};

	extern WindowSystem* WINDOWMAIN;
}
