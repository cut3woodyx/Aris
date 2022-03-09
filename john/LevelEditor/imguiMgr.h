/******************************************************************************/
/*!
\file		imguiMgr.h
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

#ifndef _SYSTEM_IMGUI_MANAGER
#define _SYSTEM_IMGUI_MANAGER

#include <Windows.h>
#include "SystemInput.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "SystemGraphics.h"
#include "SystemSerialization.h"
#include <deque>

namespace FwEngine
{
	class LESysMgr;
	class ImguiMgr
	{
		SystemInput* _input;
		SystemGraphics* _graphics;
		HWND _handle;

		LESysMgr* _mgr;

		bool _displayImGUI;

		std::string _curFile[3];

		std::set<std::string> _tagList;
		std::set<std::string> _prefabFileList;

		int _sObj;
		int _sPrefab;
		bool _playEngine;

		RECT _clientRect;

		enum Type : size_t
		{
			NONE = 0,
			TRANSLATE,
			SCALE,
			SCALECOLLISION,
			ROTATE
		};
		size_t _type;

		std::deque < std::tuple < size_t, std::string, Vector3D >> _undoTrans;

		void StartFrame();
		void EndFrame();
		void ImGUIInput(float dt);

		int ImGUIMainMenuBar();
		void ImGUIList();
		void ImGUIListComps(
			ComponentTransform& trn,
			ComponentCollision& col,
			ComponentPhysics& phy,
			ComponentRenderer& ren,
			ComponentAnimation& anim,
			ComponentText& text,
			ComponentAudio& audio,
			ComponentScript& script,
			ComponentEmitter& emitter);

		void ImGUIPerformance(float dt);

		std::string ImGUICopyFile(std::string filename, std::string newFilePath);

		void ImGUIUndo();


		void ImGUIMap();
		void ImGUIPrefab();

	public:
		typedef std::pair<std::string, std::unordered_map<std::string, ParamValueMap>> paramPair;
		bool _mouseCaptured;
		bool _mouseUsed;

		ImguiMgr();
		~ImguiMgr();

		void Init(LESysMgr* mgr);
		int Update(float dt);
		bool GetIfPlayEngine();
		void SaveToFile(std::string filename);

		std::vector<paramPair> GetAllObjectParams();
		paramPair GetObjectParam(size_t i);
	};
}

#endif // ! _SYSTEM_IMGUI_MANAGER

