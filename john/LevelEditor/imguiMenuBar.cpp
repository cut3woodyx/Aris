/******************************************************************************/
/*!
\file		imguiMenuBar.cpp
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

#include "imguiMgr.h"
#include "LESysMgr.h"
#include "WindowsDialogBox.h"
#include "SystemSerialization.h"
#include "WindowSystem.h"
#include "StringConverter.h"

namespace FwEngine
{
	namespace
	{
		Vector3D prevCamPos;

		// End Editor
		int EscLE(HWND _handle)
		{
			if (MessageBox(_handle, 
				UTF8_decode("Are you sure? All unsaved data will be lost.").c_str(), 
				UTF8_decode("Exit").c_str(), MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
				return 1;
			return 0;
		};

		// Save Message when done
		void SaveMessage(HWND _handle)
		{
			MessageBox(_handle, 
				UTF8_decode("File(s) are saved!").c_str(), 
				UTF8_decode("Saved File").c_str(), MB_OK | MB_ICONINFORMATION);
		};

		// Open windows Dialog Box to open files
		void OpenBox(LESysMgr* _mgr, HWND _handle, std::string* _curFile, 
			std::set<std::string>& _tagList, std::set<std::string>& _prefabFileList)
		{
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"*.lesav", L"*.lesav" },
				{ L"*.txt", L"*.txt" },
				{ L"*.csv", L"*.csv" },
			};
			//WINDOWMAIN->DisableClose();
			std::string filename = WindowsOpenFileBox(_handle, rgSpec, 3);

			//WINDOWMAIN->EnableClose();
			if (!filename.empty())
			{
				// Copy files
				std::string newFilePath = "\\Resources\\LEFiles";
				std::string newName = filename.substr(filename.rfind("\\"), filename.size());
				CreateDirectory(UTF8_decode(newFilePath).c_str(), NULL);
				newFilePath.append(newName);
				CopyFile(UTF8_decode(filename).c_str(), UTF8_decode(newFilePath).c_str(), false);

				std::string objfile = _curFile[1], mapfile = _curFile[2];
				// Different things depending on file type
				if (filename.rfind(".lesav") != std::string::npos)
				{
					TextSerializer srz;
					srz.OpenRead(filename);
					// Load data
					{
						_tagList.clear();
						_prefabFileList.clear();
						// Setting up lesav data
						srz.ReadStringLine(objfile); srz.ReadStringLine(mapfile);
						int val = 0;
						while (srz.IsGood())
						{
							std::string tags;
							srz.ReadStringLine(tags);
							if (tags._Equal("</>"))
							{
								++val;
								continue;
							}
							switch (val)
							{
							case 0:
								_tagList.emplace(tags);
								break;
							case 1:
								_prefabFileList.emplace(tags);
								break;
							default:
								break;
							}
						}
						if (_tagList.empty())
							_tagList.emplace("0");
					}
					_curFile[0] = filename;
				}
				else if (filename.rfind(".txt") != std::string::npos)
					objfile = filename;
				else if (filename.rfind(".csv") != std::string::npos)
					mapfile = filename;
				_mgr->Free();
				_mgr->Load(objfile, mapfile, true);
				if (!objfile.empty())
					_curFile[1] = objfile;
				if (!mapfile.empty())
					_curFile[2] = mapfile;
			}
		};

		void SaveCSV(Map& map, std::string file)
		{
			// Save Map
			CustomTextSerializer saveCSV;
			std::vector<std::string> textureArr;
			for (GLuint i : map._tileTex)
				textureArr.push_back(RESOURCE->FindFileByTextureID(i));
			saveCSV.CustomSaveMapCSV(file, map._width, map._height, map.MAP_SCALE, map._mapArray, map._mapArrayCol, textureArr);
		};

		void SaveText(ImguiMgr& imguiMgr, std::string file)
		{
			// Save objects list
			typedef std::pair<std::string, std::unordered_map<std::string, ParamValueMap>> vecPair;
			CustomTextSerializer saveObjs;
			std::vector<vecPair> fullObjMap;

			fullObjMap = imguiMgr.GetAllObjectParams();
			saveObjs.CustomSaveObjComponent(file, fullObjMap);
		};

		bool IsFullscreen(HWND windowHandle)
		{
			// Checks if fullscreen or not
			MONITORINFO monitorInfo = { 0 };
			monitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

			RECT windowRect;
			GetWindowRect(windowHandle, &windowRect);

			return windowRect.left == monitorInfo.rcMonitor.left
				&& windowRect.right == monitorInfo.rcMonitor.right
				&& windowRect.top == monitorInfo.rcMonitor.top
				&& windowRect.bottom == monitorInfo.rcMonitor.bottom;
		}

		void SaveLesav(
			std::string* _curFile, std::set<std::string>& _tagList,
			std::set<std::string>& _prefabFileList,
			ImguiMgr& _imguiMgr, Map& _map,
			std::string filename, std::string filePath)
		{
			// Save lesav file
			TextSerializer saveFiles;

			_curFile[1] = std::string(filePath).append(".txt");
			_curFile[2] = std::string(filePath).append(".csv");
			// Save data
			{
				saveFiles.Write(_curFile[1]); saveFiles.Write(_curFile[2]);
				for (std::string i : _tagList)
					saveFiles.Write(i);
				saveFiles.Write("</>");
				for (std::string i : _prefabFileList)
					saveFiles.Write(i);
			}
			saveFiles.OpenWrite(filename);
			SaveText(_imguiMgr, _curFile[1]);
			SaveCSV(_map, _curFile[2]);
			WINDOWMAIN->SaveWindow();
		};

		void SaveBox(
			HWND _handle,
			std::string* _curFile, std::set<std::string>& _tagList,
			std::set<std::string>& _prefabFileList,
			ImguiMgr& _imguiMgr, Map& _map)
		{
			// Open windows dialog box to save data
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"*.lesav", L"*.lesav" },
				{ L"*.txt", L"*.txt" },
				{ L"*.csv", L"*.csv" },
			};
			std::string filename = WindowsSaveFileBox(_handle, rgSpec, 3);
			if (filename.size() == 0)
				return;
			// File type does different things
			if (filename.rfind(".lesav") != std::string::npos)
			{
				TextSerializer saveFiles;
				_curFile[0] = filename;
				std::string filePath = filename.substr(0, filename.size() - 6);
				SaveLesav(_curFile, _tagList, _prefabFileList, _imguiMgr, _map, filename, filePath);
				SaveMessage(_handle);
			}
			else if (filename.rfind(".txt") != std::string::npos)
			{
				SaveText(_imguiMgr, filename);
				SaveMessage(_handle);
			}
			else if (filename.rfind(".csv") != std::string::npos)
			{
				SaveCSV(_map, filename);
				SaveMessage(_handle);
			}

			if (!filename.empty())
				_curFile[0] = filename;
		};

		void CtrlSaveBox(
			HWND _handle,
			std::string* _curFile, std::set<std::string>& _tagList,
			std::set<std::string>& _prefabFileList,
			ImguiMgr& _imguiMgr, Map& _map)
		{
			// ctrl+s shortcut
			if (_curFile[0].empty())
				SaveBox(_handle, _curFile, _tagList, _prefabFileList, _imguiMgr, _map);
			else
			{
				TextSerializer saveFiles;
				std::string filePath = _curFile[0].substr(0, _curFile[0].size() - 6);
				SaveLesav(_curFile, _tagList, _prefabFileList, _imguiMgr, _map, _curFile[0], filePath);
				SaveMessage(_handle);
			}
		};
	}

	int ImguiMgr::ImGUIMainMenuBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 5 });

		// Only run if not playing
		if (!_playEngine)
		{
			if (_input->GetKeyIf(IKEY_CTRL))
			{
				if (_input->GetKeyIf(IKEY_S, SystemInput::PRESS))
					CtrlSaveBox(_handle, _curFile, _tagList, _prefabFileList, *this, _mgr->_map);
				if(_input->GetKeyIf(IKEY_O, SystemInput::PRESS))
					OpenBox(_mgr, _handle, _curFile, _tagList, _prefabFileList);
			}
			if (_input->GetKeyIf(IKEY_ESCAPE, SystemInput::PRESS))
				if (EscLE(_handle))
					return 1;
		}

		/*if (_input->GetKeyIf(IKEY_CTRL, SystemInput::HOLD) && _input->GetKeyIf(IKEY_G))
			SaveWindow(FILE_PATH);*/

		// Area to dock windows in

		bool config = false;
		
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		//ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("DockSpaceBK");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//ImGui::MenuItem("(dummy menu)", NULL, false, false);
				if (!_playEngine)
				{
					if (ImGui::MenuItem("New"))
					{
						if (MessageBox(_handle,
							UTF8_decode("Are you sure? All unsaved data will be lost.").c_str(),
							UTF8_decode("New").c_str(),
							MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
						{
							for (unsigned i = 0; i < 3; ++i)
								_curFile[i] = "";
							_mgr->Free();
							_tagList.clear();
						}
					}
					if (ImGui::MenuItem("Open", "Ctrl+O"))
						OpenBox(_mgr, _handle, _curFile, _tagList, _prefabFileList);
					if (ImGui::MenuItem("Save", "Ctrl+S"))
						CtrlSaveBox(_handle, _curFile, _tagList, _prefabFileList, *this, _mgr->_map);
					if (ImGui::MenuItem("Save As.."))
						SaveBox(_handle, _curFile, _tagList, _prefabFileList, *this, _mgr->_map);
					if (ImGui::MenuItem("Exit", "Esc"))
						if (EscLE(_handle))
							return 1;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				std::string fscreen;
				//TODO
				if (ImGui::MenuItem("Configurations"))
				{
					config = true;
				}
				if (!IsFullscreen(_handle))
					fscreen = "Fullscreen";
				else
					fscreen = "Window";
				if (ImGui::MenuItem(fscreen.c_str()))
				{
					_mgr->_windows->ToggleFullScreen();
					WINDOWMAIN->SaveWindow();
				}
				ImGui::EndMenu();
			}

			if (config)
			{
				ImGui::OpenPopup("Windows Settings");
			}

			if (ImGui::BeginPopupModal("Windows Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				std::vector<std::string>& temp = WINDOWMAIN->GetResolutions();

				static int previousIndex;

				if (ImGui::BeginCombo("Resolutions##reso", temp[WINDOWMAIN->_currentResolutionIndex].c_str(), 0))
				{
					
					for (int n = 0; n < temp.size(); ++n)
					{
						bool is_selected = (WINDOWMAIN->_currentResolutionIndex == n);
						if (ImGui::Selectable(temp[n].c_str(), is_selected))
						{
							previousIndex = WINDOWMAIN->_currentResolutionIndex;
							WINDOWMAIN->_currentResolutionIndex = n;
						}
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("FullScreen Mode##fullscreen", &WINDOWMAIN->_fullScreen);
				float min = 0.0f, max = 1.0f;
				static float masterVolume = WINDOWMAIN->_masterVolume;

				ImGui::SliderScalar("Master Volume", ImGuiDataType_Float, &masterVolume, &min, &max);

				if (ImGui::Button("Ok"))
				{
					WINDOWMAIN->_masterVolume = masterVolume;
					WINDOWMAIN->SetMasterVolume();
					WINDOWMAIN->ToggleFullScreen(false);
					WINDOWMAIN->ChangeResolution();
					config = false;
					WINDOWMAIN->SaveWindow();
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					WINDOWMAIN->_currentResolutionIndex = previousIndex;
					WINDOWMAIN->_fullScreen = WINDOWMAIN->GetFullScreenState();
					config = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			
			ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);

			//if (ImGui::Selectable("PLAY", &_playEngine, 0, { 50,30 })) {}
			if (ImGui::Checkbox("PLAY", &_playEngine))
			{
				prevCamPos = _mgr->_systemGraphics._cameraPos;
				if (_playEngine) // If Playing
				{
					DeleteFile(L"./tmpObj.txt");
					DeleteFile(L"./tmpMap.csv");
					SaveText(*this, "./tmpObj.txt");
					SaveCSV(_mgr->_map, "./tmpMap.csv");
					//WINDOWMAIN->SaveWindow();
					SetFileAttributes(L"./tmpObj.txt", GetFileAttributes(L"./tmpObj.txt") | FILE_ATTRIBUTE_HIDDEN);
					SetFileAttributes(L"./tmpMap.csv", GetFileAttributes(L"./tmpMap.csv") | FILE_ATTRIBUTE_HIDDEN);
					_mgr->Free();
					_mgr->Load("./tmpObj.txt", "./tmpMap.csv", false);
				}
				//if (!_playEngine)
				else
				{
					_mgr->Free();
					_mgr->Load("./tmpObj.txt", "./tmpMap.csv", true);
					DeleteFile(L"./tmpObj.txt");
					DeleteFile(L"./tmpMap.csv");
					while (ShowCursor(true) <= 0);
				}
				_mgr->_systemGraphics._cameraPos = prevCamPos;
				_mgr->_systemGraphics.UpdateCameraView();
			}
			ImGui::SameLine(ImGui::GetWindowWidth()-80);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleVar();

		ImGui::End();

		return 0;
	}
}