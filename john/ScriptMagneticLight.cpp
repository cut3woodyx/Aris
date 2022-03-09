/******************************************************************************/
/*!
\file		ScriptMagneticLight.cpp
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "IScript.h"
#include <math.h>
#include "ScriptMagneticLight.h"
#include "SystemManager.h"

namespace FwEngine
{
	ScriptMagneticLight::ScriptMagneticLight(ComponentScript& comp) : IScript(STRING_SCRIPT_LIGHT, comp),
		calTrans(_components.getComponent<ComponentTransform>()),
		calRen { _components.getComponent<ComponentRenderer>() },
		calCol{ _components.getComponent<ComponentCollision>() },
		objectPool{ *(_components.getParentGameObject()->GetParentObjectPool()) },
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager()),
		_blastTrans{ nullptr },
		_blastCol{ nullptr },
		_blastRen{ nullptr },
		_playerCol{ nullptr },
		_initVertexes {calCol._vertexA, calCol._vertexB, calCol._vertexC, calCol._vertexD},
		_xScale {calTrans._scale.x},
		_yScale{ calTrans._scale.y }

	{
	}

	ScriptMagneticLight::~ScriptMagneticLight()
	{
	}

	void ScriptMagneticLight::Init()
	{
		ComponentTransform* blast = &objectPool.GetContainerPtr<ComponentTransform>()->at(0);

		const size_t compSize = objectPool.GetContainerPtr<ComponentTransform>()->size();
		for (size_t i = 0; i < compSize; ++blast, ++i)
		{
			if (blast->_tag == "blast")
			{
				_blastTrans = blast;
				_blastCol = &objectPool.GetContainerPtr<ComponentCollision>()->at(i);
				_blastRen = &objectPool.GetContainerPtr<ComponentRenderer>()->at(i);
				break;
			}
		}

		ComponentTransform* player = &objectPool.GetContainerPtr<ComponentTransform>()->at(0);
		for (size_t i = 0; i < compSize; ++player, ++i)
		{
			if (player->_tag == "player")
			{
				_playerCol = player;
				break;
			}
		}

		//Get rid of floating point scale
		calTrans._scale.x = (float)(int)calTrans._scale.x;
		calTrans._scale.y = (float)(int)calTrans._scale.y;
	}

	void ScriptMagneticLight::Update(float)
	{
		if (_blastTrans)
		{
			calTrans._currentPosition = _blastTrans->_currentPosition;
			calTrans._rotation = _blastTrans->_rotation;
			if (!calTrans._rotation)
				calRen._flipX = _blastRen->_flipX;
			else
				calRen._flipX = false;
			calTrans._scale.x = _xScale;
			calTrans._scale.y = _yScale;
		}

		if (calTrans._rotation)
		{
			float xScale = (calTrans._scale.x / calTrans._scale.y) / 2.0f;
			float yScale = (calTrans._scale.y / calTrans._scale.x) / 2.0f;

			calCol._vertexA = { -yScale, -xScale, 0 };
			calCol._vertexB = { yScale, xScale, 0 };
			calCol._vertexC = { yScale, -xScale, 0 };
			calCol._vertexD = { -yScale, xScale, 0 };
		}
		else
		{
			calCol._vertexA = _initVertexes[0];
			calCol._vertexB = _initVertexes[1];
			calCol._vertexC = _initVertexes[2];
			calCol._vertexD = _initVertexes[3];
		}
	}

	void ScriptMagneticLight::Free()
	{
	}

	void ScriptMagneticLight::Destroy()
	{
	}

	void ScriptMagneticLight::OnCollisionEnter(size_t)
	{
	}

	void ScriptMagneticLight::OnCollisionExit(size_t)
	{
	}

	void ScriptMagneticLight::OnCollisionStay(size_t id)
	{
		ComponentPhysics& otherComp = (*objectPool.GetContainerPtr<ComponentPhysics>())[id];
		
		if (otherComp._transformComponent)
		{
			ComponentTransform& otherTrans = (*objectPool.GetContainerPtr<ComponentTransform>())[id];
			ComponentCollision& otherCol = (*objectPool.GetContainerPtr<ComponentCollision>())[id];
			if (otherComp._transformComponent->_tag.find("metal") != std::string::npos)
			{
				if (calTrans._rotation)
				{
					float dist;

					if (_playerCol->_currentPosition.y > otherTrans._currentPosition.y)
					{
						dist = -((_blastTrans->_scale.x * (_blastCol->_vertexB.y - _blastCol->_vertexA.y) / 2.0f)
							+ (otherTrans._scale.y * (otherCol._vertexB.y - otherCol._vertexA.y) / 2.0f)
							- (_blastTrans->_currentPosition.y - otherTrans._currentPosition.y)) / 2.0f;
					}
					else
					{
						dist = ((_blastTrans->_scale.x * (_blastCol->_vertexB.y - _blastCol->_vertexA.y) / 2.0f)
							+ (otherTrans._scale.y * (otherCol._vertexB.y - otherCol._vertexA.y) / 2.0f)
							- (otherTrans._currentPosition.y - _blastTrans->_currentPosition.y)) / 2.0f;
					}

					calTrans._currentPosition.y = _blastTrans->_currentPosition.y - dist;

					calTrans._scale.x = abs(
						(_playerCol->_currentPosition.y
							- otherComp._transformComponent->_currentPosition.y
							))
						- (otherTrans._scale.y * (otherCol._vertexB.y - otherCol._vertexA.y) / 2.0f);

				}
				else
				{
					float dist;

					if (_playerCol->_currentPosition.x > otherTrans._currentPosition.x)
					{
						dist = -((_blastTrans->_scale.x * (_blastCol->_vertexB.x - _blastCol->_vertexA.x) / 2.0f)
							+ (otherTrans._scale.x * (otherCol._vertexB.x - otherCol._vertexA.x) / 2.0f)
							- (_blastTrans->_currentPosition.x - otherTrans._currentPosition.x)) / 2.0f;
					}
					else
					{
						dist = ((_blastTrans->_scale.x * (_blastCol->_vertexB.x - _blastCol->_vertexA.x) / 2.0f)
							+ (otherTrans._scale.x * (otherCol._vertexB.x - otherCol._vertexA.x) / 2.0f)
							- (otherTrans._currentPosition.x - _blastTrans->_currentPosition.x)) / 2.0f;
					}

					calTrans._currentPosition.x = _blastTrans->_currentPosition.x - dist;

					calTrans._scale.x = abs(
						(_playerCol->_currentPosition.x
							- otherComp._transformComponent->_currentPosition.x
							))
						- (otherTrans._scale.x * (otherCol._vertexB.x - otherCol._vertexA.x) / 2.0f);
				}
			}
		}
	}
}

