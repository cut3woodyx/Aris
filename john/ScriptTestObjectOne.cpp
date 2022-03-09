/******************************************************************************/
/*!
\file		ScriptTestObjectOne.cpp
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
#include "ScriptTestObjectOne.h"

//#include "SystemInput.h"
//#include "SystemGraphics.h"

#include "IScript.h"

FwEngine::ScriptTestObjectOne::ScriptTestObjectOne(ComponentScript& comp) : 
	IScript(STRING_SCRIPT_TESTOBJECTONE, comp),
	_colPhy(
		_components.getComponent<ComponentPhysics>()
	),
	_colAnimation(
		_components.getComponent<ComponentAnimation>()
	),
	_colRender(
		_components.getComponent<ComponentRenderer>()
	),
	_colTransform(
		_components.getComponent<ComponentTransform>()
	),
	_inp(
		_components.getSystemInput()
	),
	_gfx(
		_components.getSystemGraphics()
	)
{
}

FwEngine::ScriptTestObjectOne::~ScriptTestObjectOne()
{
}

void FwEngine::ScriptTestObjectOne::Init()
{
}

void FwEngine::ScriptTestObjectOne::Update(float)
{
	//

	FwMath::Vector3D mousePos = _components.getCursorPosition();

	float x = abs(mousePos.x - _colTransform._currentPosition.x);
	float y = abs(mousePos.y - _colTransform._currentPosition.y);

	if (x < 150.0f && y < 150.0f)
	{
		_colTransform._rotation = _colTransform._rotation + 5.0f;
	}

	//SystemInput& inp = _components.getSystemInput();

	//ComponentCollision& colCollision = _components.getComponent<ComponentCollision>();

	//ComponentPhysics& colPhy = _components.getComponent<ComponentPhysics>();

	//if (colPhy.isEnabled())
	//{
	//	if ((inp)(IKEY_I, SystemInput::HOLD))
	//	{
	//		colPhy.AddForce({ 0,100,0 });
	//	}
	//	if ((inp)(IKEY_J, SystemInput::HOLD))
	//	{
	//		colPhy.AddForce({ -100,0,0 });
	//	}
	//	if ((inp)(IKEY_K, SystemInput::HOLD))
	//	{
	//		colPhy.AddForce({ 0,-100,0 });
	//	}
	//	if ((inp)(IKEY_L, SystemInput::HOLD))
	//	{
	//		colPhy.AddForce({ 100,0,0 });
	//	}
	//}
}

void FwEngine::ScriptTestObjectOne::Free()
{
}

void FwEngine::ScriptTestObjectOne::Destroy()
{
}

void FwEngine::ScriptTestObjectOne::OnCollisionEnter(size_t )
{
}

void FwEngine::ScriptTestObjectOne::OnCollisionExit(size_t )
{
}

void FwEngine::ScriptTestObjectOne::OnCollisionStay(size_t)
{
}

//void FwEngine::ScriptTestObjectOne::OnTriggerEnter(int id)
//{
//}
//
//void FwEngine::ScriptTestObjectOne::OnTriggerExit(int id)
//{
//}
//
//void FwEngine::ScriptTestObjectOne::OnTriggerStay(int id)
//{
//}
