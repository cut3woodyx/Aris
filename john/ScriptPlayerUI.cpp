/******************************************************************************/
/*!
\file		ScriptPlayerUI.cpp
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
#include "ScriptPlayerUI.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "SystemManager.h"

FwEngine::ScriptPlayerUI::ScriptPlayerUI(ComponentScript& comp) :
	IScript(STRING_SCRIPT_PLAYERUI, comp),
	colPhy(_components.getComponent<ComponentPhysics>()),
	colAnimation(_components.getComponent<ComponentAnimation>()),
	colRender(_components.getComponent<ComponentRenderer>()),
	colTrans(_components.getComponent<ComponentTransform>()),
	inp(_components.getSystemInput()),
	gfx(_components.getSystemGraphics()),
	sysMgr(_components.getSystemManager()),
	health(sysMgr._playerMgr.health),
	isBlue(sysMgr._playerMgr.isBlue),
	keyAmt(sysMgr._playerMgr.keys),
	isChanging(false),
	myKey(keyAmt),
	myhealth(health),
	uiColorRed(false)
{
}

FwEngine::ScriptPlayerUI::~ScriptPlayerUI()
{
}

void FwEngine::ScriptPlayerUI::Init()
{
	colRender.enable();
	myhealth = health;
	myKey = keyAmt;

	if (colTrans._tag._Equal("KeyUI"))
	{
		colAnimation._defaultAnimationName = "";
	}

	if (colTrans._tag._Equal("Hurt"))
	{
		colRender.disable();
	}

	/*if (colTrans._tag._Equal("UIPolarity"))
	{
		colTrans._currentPosition = FwMath::Vector3D(-gfx.GetWidth() * offsetX, gfx.GetHeight() * offsetY, 0.0f);
	}

	if (colTrans._tag.substr(0, 8)._Equal("UIHealth"))
	{
		int index = std::stoi(colTrans._tag.substr(8, colTrans._tag.size()));

		colTrans._currentPosition = FwMath::Vector3D(-gfx.GetWidth() * offsetX + polarityHealthSpacing + healthSpacing * index, gfx.GetHeight() * offsetY + healthYOffset, 0.0f);
	}
*/
}


void FwEngine::ScriptPlayerUI::Update(float)
{
	
	//if (sysMgr._gamestateNext == sysMgr.GAMESTATE_LOAD_NEXTSCENE)
	//{
	//	colRender.disable();
	//	return;
	//}

	if (sysMgr._level < sysMgr.STARTLEVEL_INDEX)
	{
		colRender.disable();
		return;
	}
	
	colRender.enable();


	if (colTrans._tag._Equal("Tick1"))
	{
		if (sysMgr._playerMgr.FindLocation("TrainingRoom"))
		{
			colRender.enable();
		}

		else 
		{
			colRender.disable();
		}
	}

	if (colTrans._tag._Equal("Tick2"))
	{
		if (sysMgr._playerMgr.FindLocation("GeneratorRoom"))
		{
			colRender.enable();
		}

		else
		{
			colRender.disable();
		}
	}
	if (colTrans._tag._Equal("Tick3"))
	{
		if (sysMgr._playerMgr.FindLocation("ResearchRoom"))
		{
			colRender.enable();
		}

		else
		{
			colRender.disable();
		}
	}

	if (colTrans._tag._Equal("KeyUI")) 
	{
		if (keyAmt == 0) 
		{
			if (!colAnimation.IsPlaying())
			{
				colAnimation.PlayAnimation("card3", false);
			}
		}
		else if (keyAmt == 1)
		{
			if (!colAnimation.IsPlaying())
			{
				colAnimation.PlayAnimation("card0", false);
			}
			//colAnimation.PlayAnimation("card0", false);		
		}
		else if (keyAmt == 2)
		{
			if (!colAnimation.IsPlaying())
			{
				colAnimation.PlayAnimation("card1", false);
			}
			//colAnimation.PlayAnimation("card1", false);		
		}
		else if (keyAmt == 3)
		{
			if (!colAnimation.IsPlaying())
			{
				colAnimation.PlayAnimation("card2", false);
			}
			//colAnimation.PlayAnimation("card2", false);		
		}

	}

	

	if (colTrans._tag._Equal("Hurt")) 
	{
		if (myhealth != health)
		{
			
			
			if ((health - myhealth) > 0)
			{
				//Healing (Green)
				colRender._colour = { 0.0f, 1.0f, 0.0f, 1.0f };
			}
			else
			{
				//Take damage (Red)
				colRender._colour = { 1.0f, 0.0f, 0.0f, 1.0f };
			}
			colRender.enable();
			colAnimation.PlayAnimation("hurt", false);
			myhealth=health;
			//colAnimation.SetAnimationEnd(4);			
		}

		if (myKey != keyAmt)
		{
			colRender.enable();
			colRender._colour = { 1.0f, 1.0f, 0.0f, 1.0f };
			colAnimation.PlayAnimation("hurt", false);
			myKey = keyAmt;

			//colAnimation.SetAnimationEnd(4);			
		}
		if (sysMgr._playerMgr.isShakeEvent && !sysMgr._playerMgr.isTimerUp && myhealth == health)
		{
			colRender.enable();
			colRender._colour = { 0.0f, 0.0f, 0.0f, 1.0f };
			colAnimation.PlayAnimation("hurt", true);
	
		}
		

		if (colRender.isEnabled()|| !sysMgr._playerMgr.isShakeEvent)
		{
			if (!colAnimation.IsPlaying())
			{
				colRender.disable();
				colAnimation.StopAnimation();
			}
		}
	}


	if (colTrans._tag._Equal("EndGame"))
	{

		if (sysMgr._playerMgr.isTimerUp)
		{
			colRender.enable();

			//colRender._colour = { 0,0,0, 1 / sysMgr._playerMgr.health * 0.2 };

			
				float value = (5- sysMgr._playerMgr.health)*0.25f;
				if (sysMgr._playerMgr.health >0)
				colRender._colour = { 0,0,0, value };

				
			
			

			colAnimation.PlayAnimation("Default", true);


		}
		else
		{
			colRender.disable();
		}
	}

	if (colTrans._tag._Equal("UIPolarity"))
	{
		if ((inp)(IMOUSE_LBUTTON, SystemInput::PRESS))
		{		
			isChanging = true;		
		}

		if ((inp)(IMOUSE_RBUTTON, SystemInput::PRESS))
		{			
			isChanging = true;
		}

		if(isBlue)
		{
			if (!isChanging) 
			{
				colAnimation.PlayAnimation("blue_idle", true);
				uiColorRed = false;
			}
				
			else if(isChanging && uiColorRed == true)
			{
				colAnimation.PlayAnimation("red_change", false);
				if (!colAnimation.IsPlaying()) 
				{
					isChanging = false;
				}	
			}			
		}
	
		else
		{
			if (!isChanging)
			{
				colAnimation.PlayAnimation("red_idle", true);
				uiColorRed = true;
			}

			else if (isChanging && uiColorRed == false)
			{
				colAnimation.PlayAnimation("blue_change", false);
				if (!colAnimation.IsPlaying())
				{
					isChanging = false;
				}
			}
		}
				
	}

	if (colTrans._tag.substr(0, 8)._Equal("UIHealth"))
	{
		int index = std::stoi(colTrans._tag.substr(8, colTrans._tag.size()));
		if (index >= health)
			colRender.disable();
	}
}

void FwEngine::ScriptPlayerUI::Unload()
{
	colRender.enable();
}

void FwEngine::ScriptPlayerUI::Free()
{
}

void FwEngine::ScriptPlayerUI::Destroy()
{
}

void FwEngine::ScriptPlayerUI::OnCollisionEnter(size_t )
{
}

void FwEngine::ScriptPlayerUI::OnCollisionExit(size_t )
{
}

void FwEngine::ScriptPlayerUI::OnCollisionStay(size_t )
{
}


