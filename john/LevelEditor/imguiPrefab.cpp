/******************************************************************************/
/*!
\file		imguiPrefab.cpp
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
#include "imgui_internal.h"
#include "WindowsDialogBox.h"
#include "LESysMgr.h"
#include "StringConverter.h"

namespace FwEngine
{
	namespace 
	{
		static bool makePrefab = false;
		static std::string fileToMake;
		static GameObject* objToStore = nullptr;
	}
	std::string getFileName(std::string file)
	{
		if (file.rfind('\\') != std::string::npos && file.rfind('.') != std::string::npos)
		{
			std::string re = file.substr(file.rfind('\\') + 1, file.size()-1);
			re = re.substr(0, re.rfind('.'));
			return re;
		}
		return {};
	}
	void ImguiMgr::ImGUIPrefab()
	{
		//ImGui::SetNextWindowSize({ _clientRect.right * 0.15f, _clientRect.bottom *0.25f }, ImGuiCond_Always);
		//ImGui::SetNextWindowPos({ _clientRect.right * 0.55f, _clientRect.bottom * 0.70f });

		ImGui::Begin("Prefabs", nullptr);//, (bool*)0,
			//ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar
			//| ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::IsWindowCollapsed())
		{
			ImGui::End();
			return;
		}


		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowContentRegionMax();
		ImVec2 pad = ImGui::GetCurrentWindow()->WindowPadding;
		if (ImGui::BeginDragDropTargetCustom(
			ImRect{ winPos.x + pad.x, winPos.y + pad.y, 
			winSize.x + winPos.x, winSize.y + winPos.y },
			ImGui::GetCurrentWindowRead()->ID))
		{
			// If dropped into Prefab Window
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB_ADDDROP"))
			{
				IM_ASSERT(payload->DataSize == sizeof(std::string));
				objToStore = (GameObject*)payload->Data;
			}
		}

		if (objToStore)
		{
			// Create a single object file
			CustomTextSerializer saveObjs;
			size_t i = objToStore->GetIndex();
			std::vector<paramPair> fullObjMap;

			paramPair singleObj = GetObjectParam(i);
			fullObjMap.push_back(singleObj);

			if (!saveObjs.OpenRead(".\\Resources\\Prefabs"))
				CreateDirectory(L".\\Resources\\Prefabs", NULL);
			std::string filePath = std::string(".\\Resources\\Prefabs\\").append(singleObj.first).append(".efab").c_str();
			saveObjs.CustomSaveObjComponent(
				filePath,
				fullObjMap);
			_prefabFileList.emplace(filePath);
			objToStore = nullptr;
		}

		// Manual addition of file
		if (ImGui::Button("Add Prefab"))
		{
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"*.txt", L"*.txt" },
				{ L"*.efab", L"*.efab" },
			};
			// Open to set texture
			//WINDOWMAIN->DisableClose();
			std::string filename = WindowsOpenFileBox(_handle, rgSpec, 2);
			if (!filename.empty())
			{
				filename = ImGUICopyFile(filename, ".\\Resources\\Prefabs");
				_prefabFileList.emplace(filename);
			}
		}
		ImGui::SameLine();
		// Remove/Delete
		if (ImGui::Button("Remove Prefab"))
			if (_sPrefab >= 0)
				ImGui::OpenPopup("Delete Object?");
		if (_input->GetKeyIf(IKEY_DELETE, SystemInput::HOLD))
			if (_sPrefab >= 0)
				ImGui::OpenPopup("Delete Object?");

		if (ImGui::BeginPopupModal("Delete Object?", NULL))
		{
			ImGui::Text("Are you certain? Deleted Objects may not be recoverable.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK"))
			{
				decltype(_prefabFileList)::iterator remove = std::begin(_prefabFileList);
				std::advance(remove, _sPrefab);

				//DeleteFile(UTF8_decode(*remove).c_str());
				_prefabFileList.erase(remove);
				_sPrefab = -1;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		// Sets object data to be created
		ImGui::BeginChild("##PrefabList",
			ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 60.0f), true, ImGuiWindowFlags_HorizontalScrollbar);
		int num = 0;
		for (std::string i : _prefabFileList)
		{
			if (ImGui::Selectable(getFileName(i).c_str(), num == _sPrefab, ImGuiSelectableFlags_PressedOnClick))
			{
				fileToMake = i;
				_sPrefab = num;
				_sObj = -1;
				makePrefab = true;
			}
			++num;
			/*
			if (ImGui::BeginDragDropTargetCustom(ImGui::FindWindowByName("Prefabs")->TitleBarRect(), ImGui::FindWindowByName("Prefabs")->ID))
			{
				ImGui::SetDragDropPayload("PREFAB_DRAGNDROP", (void*)i.c_str(), sizeof(std::string));
				ImGui::Text(getFileName(i).c_str());
				//ImGui::EndDragDropSource();
			}
			*/

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("PREFAB_DRAGNDROP", (void*)i.c_str(), sizeof(std::string));
				ImGui::Text(getFileName(i).c_str());
				ImGui::EndDragDropSource();
			}
		}

		// If released over location, creates prefabbed object
		if (_input->GetKeyIf(IMOUSE_LBUTTON, SystemInput::RELEASE))
		{
			if (makePrefab && !_mouseCaptured)
			{
				GameObject* obj = _mgr->_gameObjectFactory.CreateSingleSerializedGameObject(fileToMake);
				if (obj)
				{
					CursorPos mouse = _input->GetMouseWorldPos(_graphics->_cameraPos.x,
						_graphics->_cameraPos.y, _graphics->GetHeight(), _graphics->GetWidth(), _graphics->_zoom);
					Vector3D newPos = { mouse.x, mouse.y, 0 };
					(*_mgr->_gameObjectPool.GetContainerPtr<ComponentTransform>()).at(obj->GetIndex())._currentPosition
						= newPos;
					_sObj = (int)obj->GetIndex();
				}
				else
				{
					decltype(_prefabFileList)::iterator remove = std::begin(_prefabFileList);
					while (!remove->_Equal(fileToMake))
						++remove;

					if (remove != std::end(_prefabFileList))
					{
						_prefabFileList.erase(remove);
						_sPrefab = -1;
					}
				}
			}
			fileToMake.clear();
			makePrefab = false;
		}

		ImGui::EndChild();
		ImGui::End();
	}
}