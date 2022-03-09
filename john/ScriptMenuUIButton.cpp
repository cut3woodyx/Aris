/******************************************************************************/
/*!
\file		ScriptMenuUIButton.cpp
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
#include "ScriptMenuUIButton.h"
#include "SystemManager.h"

namespace FwEngine
{
	ScriptMenuUIButton::ScriptMenuUIButton(ComponentScript& comp) : IScript(STRING_SCRIPT_MENUUIBUTTON, comp),
		colRender(_components.getComponent<ComponentRenderer>()),
		colTrans(_components.getComponent<ComponentTransform>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager()),
		colAnimation(_components.getComponent<ComponentAnimation>())
	{
	}

	ScriptMenuUIButton::~ScriptMenuUIButton()
	{
	}

	void ScriptMenuUIButton::Init()
	{

		_verticeA = FwMath::Vector3D(colTrans._currentPosition.x * gfx.GetWidth() - colTrans._scale.x * 0.5f * gfx.GetWidth(), colTrans._currentPosition.y * gfx.GetHeight() - colTrans._scale.y * 0.5f * gfx.GetHeight(), 0.0f);
		_verticeB = FwMath::Vector3D(colTrans._currentPosition.x * gfx.GetWidth() + colTrans._scale.x * 0.5f * gfx.GetWidth(), colTrans._currentPosition.y * gfx.GetHeight() + colTrans._scale.y * 0.5f * gfx.GetHeight(), 0.0f);

		//if (sysMgr._level >= sysMgr.STARTLEVEL_INDEX)
		//{
		//	colRender.disable();
		//}
		colRender.disable();
	}

	bool ScriptMenuUIButton::isWithinBounds(FwMath::Vector3D cursor, FwMath::Vector3D verticeA, FwMath::Vector3D verticeB)
	{
		return cursor.x > verticeA.x && cursor.x < verticeB.x && cursor.y > verticeA.y && cursor.y < verticeB.y;
	}

	void ScriptMenuUIButton::options(FwMath::Vector3D menuCursorPos)
	{
		if (colTrans._tag._Equal("MenuOptions"))
		{
			colRender.enable();
		}

		if (colTrans._tag._Equal("OptionsMenuItemBack"))
		{
			colRender.enable();
			if (isWithinBounds(menuCursorPos, _verticeA, _verticeB))
			{
				colRender.SetTexture("Resources/UI/Back Hover.png");
				if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
				{
					sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
				}
			}
			else
			{
				colRender.SetTexture("Resources/UI/Back Unhover.png");
			}
		}
	}


	void ScriptMenuUIButton::credits(FwMath::Vector3D menuCursorPos)
	{
		if (colTrans._tag._Equal("MenuCredits"))
		{
			colRender.enable();
		}

		if (colTrans._tag._Equal("OptionsMenuItemBack"))
		{
			colRender.enable();
			if (isWithinBounds(menuCursorPos, _verticeA, _verticeB))
			{
				colRender.SetTexture("Resources/UI/Back Hover.png");
				if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
				{
					sysMgr._menustateNext = sysMgr.MENUSTATE_MAIN;
				}
			}
			else
			{
				colRender.SetTexture("Resources/UI/Back Unhover.png");
			}
		}
	}

	void ScriptMenuUIButton::Update(float)
	{
		FwMath::Vector3D curPos = sysMgr._cursorPosition;

		if (sysMgr._gamestate == sysMgr.GAMESTATE_LOAD_LEVEL)
		{
			colRender.disable();
			if (colTrans._tag._Equal("LoadingBg"))
			{
				colRender.enable();
				
			}
			return;
		}

		if (sysMgr._level == sysMgr.MAINMENU_INDEX)
		{
			
			colRender.disable();

			if (sysMgr._menustate == sysMgr.MENUSTATE_MAIN)
			{
				if (colTrans._tag._Equal("MenuItemStart"))
				{
					colRender.enable();
					if (isWithinBounds(curPos, _verticeA, _verticeB))
					{
						colRender.SetTexture("Resources/UI/Start Hover.png");
						if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
						{
							sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_LEVEL;
						}
					}
					else
					{
						colRender.SetTexture("Resources/UI/Start Unhover.png");
					}
				}

				if (colTrans._tag._Equal("MenuItemOptions"))
				{
					colRender.enable();
					if (isWithinBounds(curPos, _verticeA, _verticeB))
					{
						colRender.SetTexture("Resources/UI/Options Hover.png");
						if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
						{
							sysMgr._menustateNext = sysMgr.MENUSTATE_OPTIONS;
						}
					}
					else
					{
						colRender.SetTexture("Resources/UI/Options Unhover.png");
					}
				}

				if (colTrans._tag._Equal("MenuItemCredits"))
				{
					colRender.enable();
					if (isWithinBounds(curPos, _verticeA, _verticeB))
					{
						colRender.SetTexture("Resources/UI/Credits Hover.png");
						if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
						{
							sysMgr._menustateNext = sysMgr.MENUSTATE_CREDITS;
						}
					}
					else
					{
						colRender.SetTexture("Resources/UI/Credits Unhover.png");
					}
				}

				if (colTrans._tag._Equal("MenuItemQuit"))
				{
					colRender.enable();
					if (isWithinBounds(curPos, _verticeA, _verticeB))
					{
						colRender.SetTexture("Resources/UI/Quit Hover.png");
						if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
						{
							sysMgr._gamestateNext = sysMgr.GAMESTATE_QUIT;
						}
					}
					else
					{
						colRender.SetTexture("Resources/UI/Quit Unhover.png");
					}
				}
			}
			
			if (sysMgr._menustate == sysMgr.MENUSTATE_OPTIONS)
			{
				options(curPos);
			}

			if (sysMgr._menustate == sysMgr.MENUSTATE_CREDITS)
			{
				credits(curPos);
			}
		}
		else if (sysMgr._level == sysMgr.ANOTHERMENU_INDEX)
		{
			colRender.disable();

			if (colTrans._tag._Equal("MenuItemBack"))
			{
				colRender.enable();
				if (isWithinBounds(curPos, _verticeA, _verticeB))
				{
					colRender.SetTexture("Resources/UI/Back Hover.png");
					if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
					{
						sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_MAINMENU;
					}
				}
				else
				{
					colRender.SetTexture("Resources/UI/Back Unhover.png");
				}
			}
		}
		else
		{


			//if (sysMgr._gamestate != sysMgr.GAMESTATE_PAUSE)
			//{
			//	colRender.disable();
			//}

			colRender.disable();


			if (sysMgr._menustate == sysMgr.MENUSTATE_PLAYERDIED)
			{
				
				if (colTrans._tag._Equal("MenuYouDied"))
				{
					colRender.enable();

					colAnimation.PlayAnimation("Default", true);
					
				}
			}

			if (sysMgr._menustate == sysMgr.MENUSTATE_PLAYERWON)
			{
			
				if (colTrans._tag._Equal("MenuYouWon"))
				{
					//colRender.enable(); // enable after adding render img
				}
			}

			if (sysMgr._gamestate == sysMgr.GAMESTATE_PAUSE)
			{
				CursorPos pos =
					inp.GetMouseWorldPos(gfx._cameraPos.x,
						gfx._cameraPos.y, gfx.GetHeight(),
						gfx.GetWidth(), 1.0f);
				FwMath::Vector3D menuCursorPos = FwMath::Vector3D(pos.x, pos.y, 0) - gfx._cameraPos;

				if (sysMgr._menustate == sysMgr.MENUSTATE_MAIN)
				{
					if (colTrans._tag._Equal("MenuPause"))
					{
						colRender.enable();
					}

					if (colTrans._tag._Equal("PauseMenuItemResume"))
					{
						colRender.enable();
						if (isWithinBounds(menuCursorPos, _verticeA, _verticeB))
						{
							colRender.SetTexture("Resources/UI/Resume Hover.png");
							if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
							{
								sysMgr._gamestateNext = sysMgr.GAMESTATE_UPDATE;
							}
						}
						else
						{
							colRender.SetTexture("Resources/Sprites/Renderer/Resume Unhover.png");
						}
					}

					if (colTrans._tag._Equal("PauseMenuItemOptions"))
					{
						colRender.enable();
						if (isWithinBounds(menuCursorPos, _verticeA, _verticeB))
						{
							colRender.SetTexture("Resources/UI/Options Hover.png");
							if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
							{
								
								sysMgr._menustateNext = sysMgr.MENUSTATE_OPTIONS;
							}
						}
						else
						{
							colRender.SetTexture("Resources/Sprites/Renderer/Options Unhover.png");
						}
					}

					if (colTrans._tag._Equal("PauseMenuItemQuit"))
					{
						colRender.enable();
						if (isWithinBounds(menuCursorPos, _verticeA, _verticeB))
						{
							colRender.SetTexture("Resources/UI/Quit Hover.png");
							if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
							{
								sysMgr._gamestateNext = sysMgr.GAMESTATE_QUIT;
								//sysMgr._gamestateNext = sysMgr.GAMESTATE_LOAD_RETURNTOMENU;
							}
						}
						else
						{
							colRender.SetTexture("Resources/Sprites/Renderer/Quit Unhover.png");
						
						}
					}
				}
				else if (sysMgr._menustate == sysMgr.MENUSTATE_OPTIONS)
				{
					options(menuCursorPos);
				}

			}

		}
	
	}

	void ScriptMenuUIButton::Unload()
	{
		colRender.enable();
	}

	void ScriptMenuUIButton::Free()
	{
	}

	void ScriptMenuUIButton::Destroy()
	{
	}

	void ScriptMenuUIButton::OnCollisionEnter(size_t)
	{
	}

	void ScriptMenuUIButton::OnCollisionExit(size_t)
	{
	}

	void ScriptMenuUIButton::OnCollisionStay(size_t)
	{
	}
}

