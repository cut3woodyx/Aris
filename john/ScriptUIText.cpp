/******************************************************************************/
/*!
\file		ScirptUIText.cpp
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "ScriptPlayer.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "ScriptUIText.h"
#include "SystemManager.h"

namespace FwEngine
{
	ScriptUIText::ScriptUIText(ComponentScript& comp) : IScript(STRING_SCRIPT_UITEXT, comp),
		colText(_components.getComponent<ComponentText>()),
		colTrans(_components.getComponent<ComponentTransform>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager())
	{
	}

	ScriptUIText::~ScriptUIText()
	{
	}

	void ScriptUIText::Init()
	{
		
	}

	void ScriptUIText::Update(float)
	{
		//
		if (colTrans._tag._Equal("FPSText"))
		{
			if (sysMgr._isDebug)
			{
				//colTrans._currentPosition = gfx._cameraPos + FwMath::Vector3D(gfx.GetWidth() * 0.5f - gfx.GetWidth() * 0.1f, gfx.GetHeight() * 0.5f - gfx.GetHeight() * 0.05f, 0.0f) * gfx._zoom;
				colTrans._currentPosition = gfx._cameraPos + FwMath::Vector3D(gfx.GetWidth() * 0.5f - gfx.GetWidth() * 0.1f, gfx.GetHeight() * 0.5f - gfx.GetHeight() * 0.05f, 0.0f) * gfx._zoom;
				colText._displayText = std::string("FPS:") + std::to_string(sysMgr._currentFPS);
			}
			else
			{
				colText._displayText = "";
			}
		}
		if (colTrans._tag._Equal("OBJText"))
		{
			if (sysMgr._isDebug)
			{
				colTrans._currentPosition = gfx._cameraPos + FwMath::Vector3D(gfx.GetWidth() * 0.5f - gfx.GetWidth() * 0.2f, gfx.GetHeight() * 0.5f - gfx.GetHeight() * 0.05f, 0.0f) * gfx._zoom;
				colText._displayText = std::string("OBJ:") + std::to_string(sysMgr._currentObj);
			}
			else
			{
				colText._displayText = "";
			}
		}
	}

	void ScriptUIText::Free()
	{
	}

	void ScriptUIText::Destroy()
	{
	}

	void ScriptUIText::OnCollisionEnter(size_t)
	{
	}

	void ScriptUIText::OnCollisionExit(size_t)
	{
	}

	void ScriptUIText::OnCollisionStay(size_t)
	{
	}
}

