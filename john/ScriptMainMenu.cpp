/******************************************************************************/
/*!
\file		ScriptMainMenu.cpp
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
#include "ScriptMainMenu.h"

namespace FwEngine
{
	ScriptMainMenu::ScriptMainMenu(ComponentScript& comp) : IScript(STRING_SCRIPT_MAINMENU, comp),
		colRender(_components.getComponent<ComponentRenderer>()),
		colTrans(_components.getComponent<ComponentTransform>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics())
	{
	}

	ScriptMainMenu::~ScriptMainMenu()
	{
	}

	void ScriptMainMenu::Init()
	{
		//int wid = gfx.GetWidth();
		//int ht = gfx.GetHeight();



		gfx._cameraPos = colTrans._currentPosition + FwMath::Vector3D{ 0.0f, 0.0f, 0.0f };
		gfx._cameraPos.z = 10.0f;
		gfx._zoom = 1.0f;
		gfx.UpdateZoom();
		gfx.UpdateCameraView();


		//RECT rect;
		//GetClientRect(gfx.GetHandle(), &rect);
		//gfx.UpdateProjection(rect.right, rect.bottom);
	}

	void ScriptMainMenu::Update(float)
	{

		//int wid = gfx.GetWidth();
		//int ht = gfx.GetHeight();

		//colRender._layer = UI;
		//int i = 1;
	}

	void ScriptMainMenu::Free()
	{
	}

	void ScriptMainMenu::Destroy()
	{
	}

	void ScriptMainMenu::OnCollisionEnter(size_t)
	{
	}

	void ScriptMainMenu::OnCollisionExit(size_t)
	{
	}

	void ScriptMainMenu::OnCollisionStay(size_t)
	{
	}
}

