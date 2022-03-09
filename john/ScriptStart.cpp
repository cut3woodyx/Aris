/******************************************************************************/
/*!
\file		ScriptStart.h
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
#include "ScriptStart.h"


FwEngine::ScriptStart::ScriptStart(ComponentScript& comp): 
IScript(STRING_SCRIPT_START, comp), 
sysMgr(_components.getSystemManager()),
comAudio(_components.getComponent<ComponentAudio>())
{

}

FwEngine::ScriptStart::~ScriptStart()
{
}

void FwEngine::ScriptStart::Init()
{

	
	comAudio.Play("BGM", true);

}

void FwEngine::ScriptStart::Update(float)
{
	if (sysMgr._playerMgr.isShakeEvent) 
	{
		
		comAudio.Play("win", true);


		
	}
	
	else 
	{
		comAudio.Play("BGM", true);
	}


}

void FwEngine::ScriptStart::Free()
{
	comAudio.StopSound();
}

void FwEngine::ScriptStart::Destroy()
{
}

void FwEngine::ScriptStart::OnCollisionEnter(size_t)
{
}

void FwEngine::ScriptStart::OnCollisionExit(size_t)
{
}

void FwEngine::ScriptStart::OnCollisionStay(size_t)
{
}
