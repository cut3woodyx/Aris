/******************************************************************************/
/*!
\file		imguiMgr.cpp
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
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#include "SystemGraphics.h"
#include "LESysMgr.h"

namespace FwEngine
{
	ImguiMgr::ImguiMgr() : _input{ nullptr }, _handle{}, _graphics{ nullptr }, 
		_mgr{ nullptr }, _curFile{ "" }, _sObj{ -1 }, _mouseCaptured{ false },
		_playEngine{ false }, _type{ 0 }, _mouseUsed{ false }, _tagList{}, 
		_prefabFileList{}, _displayImGUI{ true }, _clientRect{}, _sPrefab{}
	{
	}

	ImguiMgr::~ImguiMgr()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		ImGui_ImplWin32_Shutdown();
	}

	void ImguiMgr::Init(LESysMgr* mgr)
	{
		_input = &mgr->_systemInput;
		_handle = mgr->_windows->GetWinHandle();
		_graphics = &mgr->_systemGraphics;
		_mgr = mgr;
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureMouse = 1;
		io.WantCaptureKeyboard = 1;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigDockingWithShift = false;
		//Init Win32
		ImGui_ImplWin32_Init(_handle);

		//Init OpenGL Imgui Implementation
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Setup style
		ImGui::StyleColorsClassic();
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
	}

	void ImguiMgr::StartFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		_mouseCaptured = io.WantCaptureMouse;

		GetClientRect(_handle, &_clientRect);
	}

	int ImguiMgr::Update(float dt)
	{
		GetClientRect(_handle, &_clientRect);

		ImGUIInput(dt);

		StartFrame();
		//ImGui::Begin("Dear ImGui Demo");

		//ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f,0.1f,0.1f,1.0f });

		if (_displayImGUI)
		{
			if (ImGUIMainMenuBar())
				return 1;

			ImGUIPrefab();

			ImGUIList();

			ImGUIMap();

			ImGUIUndo();

			LARGE_INTEGER timer;
			QueryPerformanceCounter(&timer);
			_mgr->_systemFuncTime.emplace_back("ImGUI", timer);

			ImGUIPerformance(dt);
		}

		//ImGui::End();
		ImGui::PopStyleColor();

		EndFrame();

		return 0;
	}
	bool ImguiMgr::GetIfPlayEngine()
	{
		return _playEngine;
	}
	void ImguiMgr::EndFrame()
	{
		// Rendering
		ImGui::EndFrame();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	std::vector<ImguiMgr::paramPair> ImguiMgr::GetAllObjectParams()
	{
		std::vector<paramPair> fullObjMap;
		GameObjectPool pool = _mgr->_gameObjectPool;

		std::vector<GameObject> objPool =  *pool.GetContainerPtr<GameObject>();

		for (size_t i = 0; i < objPool.size(); ++i)
		{
			if (!objPool[i]._isActive)
				continue;
			std::unordered_map<std::string, ParamValueMap> objMap;
			std::vector<IComponent*> comps;

			comps.push_back(&(*pool.GetContainerPtr<ComponentTransform>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentCollision>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentPhysics>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentRenderer>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentText>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentScript>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentAnimation>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentAudio>())[i]);
			comps.push_back(&(*pool.GetContainerPtr<ComponentEmitter>())[i]);

			for (IComponent* c : comps)
			{
				if (c->isEnabled())
					objMap.emplace(c->GetParams());
			}
			if (!objPool[i].GetName().empty())
			{
				if (objPool[i].GetName().find("_Menu_") != std::string::npos)
					continue;
				fullObjMap.push_back(paramPair(objPool[i].GetName(), objMap));
			}
			else
				fullObjMap.push_back(paramPair(std::string("Object").append(std::to_string(i)), objMap));
		}
		return fullObjMap;
	}

	ImguiMgr::paramPair ImguiMgr::GetObjectParam(size_t i)
	{
		GameObjectPool pool = _mgr->_gameObjectPool;

		std::vector<GameObject> objPool = *pool.GetContainerPtr<GameObject>();

		std::unordered_map<std::string, ParamValueMap> objMap;
		std::vector<IComponent*> comps;

		comps.push_back(&(*pool.GetContainerPtr<ComponentTransform>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentCollision>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentPhysics>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentRenderer>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentText>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentScript>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentAnimation>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentAudio>())[i]);
		comps.push_back(&(*pool.GetContainerPtr<ComponentEmitter>())[i]);

		for (IComponent* c : comps)
		{
			if (c->isEnabled())
				objMap.emplace(c->GetParams());
		}

		if (!objPool[i].GetName().empty())
			return (paramPair(objPool[i].GetName(), objMap));
		else
			return (paramPair(std::string("Object").append(std::to_string(i)), objMap));
	}
}