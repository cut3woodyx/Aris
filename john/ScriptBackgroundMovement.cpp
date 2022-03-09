#include "ScriptBackgroundMovement.h"
#include "SystemInput.h"
#include "SystemGraphics.h"
#include "SystemManager.h"

namespace FwEngine
{
	ScriptBackgroundMovement::ScriptBackgroundMovement(ComponentScript& comp) : IScript(STRING_SCRIPT_BACKGROUND, comp),
		objectPool{ *(_components.getParentGameObject()->GetParentObjectPool()) },
		playerTrans(_components.getComponent<ComponentTransform>()),
		inp(_components.getSystemInput()),
		gfx(_components.getSystemGraphics()),
		sysMgr(_components.getSystemManager()),
		_leftBackground{ nullptr },
		_rightBackground{ nullptr }
	{
	}
	ScriptBackgroundMovement::~ScriptBackgroundMovement()
	{
	}
	void ScriptBackgroundMovement::Init()
	{
		ComponentTransform* bg = &objectPool.GetContainerPtr<ComponentTransform>()->at(0);
		const size_t compSize = objectPool.GetContainerPtr<ComponentTransform>()->size();
		for (size_t i = 0; i < compSize; ++bg, ++i)
		{
			if (bg->_tag == "LeftBackground")
			{
				_leftBackground = bg;
			}
			else if (bg->_tag == "RightBackground")
			{
				_rightBackground = bg;
			}

			if (_leftBackground && _rightBackground)
			{
				break;
			}
		}
	}
	void ScriptBackgroundMovement::Update(float dt)
	{
		float leftBound = playerTrans._currentPosition.x - gfx.GetWidth() * gfx._zoom;
		float rightBound = playerTrans._currentPosition.x + gfx.GetWidth() * gfx._zoom;

		_leftBackground->_scale.x = gfx.GetWidth() * gfx._zoom;
		_leftBackground->_scale.y = gfx.GetHeight() * gfx._zoom;
		_rightBackground->_scale.x = gfx.GetWidth() * gfx._zoom;
		_rightBackground->_scale.y = gfx.GetHeight() * gfx._zoom;

		if (_leftBackground->_currentPosition.x < leftBound)
		{
			_leftBackground->_currentPosition.x += _leftBackground->_scale.x * 2.0f;
		}
		else if (_rightBackground->_currentPosition.x < leftBound)
		{
			_rightBackground->_currentPosition.x += _rightBackground->_scale.x * 2.0f;
		}

		if (_rightBackground->_currentPosition.x > rightBound)
		{
			_rightBackground->_currentPosition.x -= _rightBackground->_scale.x * 2.0f;
		}
		else if (_leftBackground->_currentPosition.x > rightBound)
		{
			_leftBackground->_currentPosition.x -= _leftBackground->_scale.x * 2.0f;
		}

		if ((inp)(IKEY_D, SystemInput::HOLD))
		{
			_leftBackground->_currentPosition.x -= 10.0f;
			_rightBackground->_currentPosition.x -= 10.0f;
		}
	}
	void ScriptBackgroundMovement::Free()
	{
	}
	void ScriptBackgroundMovement::Destroy()
	{
	}
	void ScriptBackgroundMovement::OnCollisionEnter(size_t)
	{
	}
	void ScriptBackgroundMovement::OnCollisionExit(size_t)
	{
	}
	void ScriptBackgroundMovement::OnCollisionStay(size_t)
	{
	}
}