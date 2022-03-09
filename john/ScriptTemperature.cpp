#include "ScriptTemperature.h"
#include "Blast.h"

namespace FwEngine
{
	ScriptTemperature::ScriptTemperature(ComponentScript& comp)
		: IScript(STRING_SCRIPT_TEMPERATURE, comp), _modifier{},
		_objectPool{ *(_components.getParentGameObject()->GetParentObjectPool()) }
	{
		AddFloatVar("Modifier", &_modifier);
	}

	ScriptTemperature::~ScriptTemperature()
	{
	}
	void ScriptTemperature::Init()
	{
	}

	void ScriptTemperature::Update(float)
	{
	}

	void ScriptTemperature::Free()
	{
	}

	void ScriptTemperature::Destroy()
	{
	}

	void ScriptTemperature::OnCollisionEnter(size_t id)
	{
		ComponentScript& scr = _objectPool.GetContainerPtr<ComponentScript>()->at(id);
		if (scr.isEnabled())
		{
			ScriptBlast* pScr = reinterpret_cast<ScriptBlast*>(scr.FindScript(STRING_SCRIPT_BLAST));
			if (pScr)
			{
				pScr->_magneticStrength = _modifier;
			}
		}
	}

	void ScriptTemperature::OnCollisionExit(size_t id)
	{
		ComponentScript& scr = _objectPool.GetContainerPtr<ComponentScript>()->at(id);
		if (scr.isEnabled())
		{
			ScriptBlast* pScr = reinterpret_cast<ScriptBlast*>(scr.FindScript(STRING_SCRIPT_BLAST));
			if (pScr)
			{
				pScr->_magneticStrength = 1.0f;
			}
		}
	}

	void ScriptTemperature::OnCollisionStay(size_t)
	{
	}

	void ScriptTemperature::PushVars()
	{
		if (_originalVars.size())
		{
			_modifier = std::stof(_originalVars[0]);
			_originalVars.erase(_originalVars.begin());
		}
	}
}
