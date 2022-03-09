/******************************************************************************/
/*!
\file		imguiUndo.cpp
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

#include "imgui.h"
#include "imguiMgr.h"
#include "LESysMgr.h"

namespace FwEngine
{
	void ImguiMgr::ImGUIUndo()
	{
		// Sets component data to previous data
		if (_input->GetKeyIf(IKEY_CTRL))
		{
			std::vector<ComponentTransform>* trnPool = _mgr->_gameObjectPool.GetContainerPtr<ComponentTransform>();
			
			if (_undoTrans.size() && _input->GetKeyIf(IKEY_Z, SystemInput::PRESS))
			{
				std::tuple<size_t, std::string, Vector3D> prev = _undoTrans.back();
				_undoTrans.pop_back();

				std::string type = (std::get<1>(prev));
				if (type == "translate")
				{
					(*trnPool)[std::get<0>(prev)]._currentPosition = std::get<2>(prev);
				}
				else if (type == "scale")
				{
					(*trnPool)[std::get<0>(prev)]._scale = std::get<2>(prev);
				}
				else if (type == "rotate")
				{
					(*trnPool)[std::get<0>(prev)]._rotation = std::get<2>(prev).x;
				}
			}
		}
	}
}