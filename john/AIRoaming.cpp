#include "AIRoaming.h"
#include "FWMath.h"

namespace FwEngine
{
	float AISpeed_ = 10000;

	AIRoaming::AIRoaming(AIScript* ai, ComponentScript* scr, std::string str) 
		: AIBase("Roaming", ai, scr, str), 
		_compPhy{ nullptr }, _playerCompTrn{ nullptr }, _distance{ 0 }, _timePassed{ 0 }, _right{ true }, _hitObj{ false }
	{
		if (_compScript)
		{
			_compPhy = &_compScript->getComponent<ComponentPhysics>();

			std::vector<ComponentTransform>& compTrans
				= *_compScript->getParentGameObject()->GetParentObjectPool()->GetContainerPtr<ComponentTransform>();

			std::vector<ComponentTransform>::iterator it =
				std::find_if(std::begin(compTrans), std::end(compTrans),
					[](ComponentTransform& comp)
					{
						return (comp._tag == "player");
					});
			if (it != std::end(compTrans))
			{
				_playerCompTrn =
					&_compScript->getParentGameObject()->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at
					(
						it->getParentGameObject()->GetIndex()
					);
			}
		}
	}
	AIRoaming::~AIRoaming()
	{
	}
	void AIRoaming::Init()
	{
		if (!_variable.empty())
			_distance = std::stof(_variable);
	}
	void AIRoaming::Update(float dt)
	{
		_timePassed += dt;
		if (_timePassed <= 1)
		{
			if (_right)
				_compPhy->AddForce(MagneticForce(Vector3D{ 1,0,0 }, AISpeed_, dt));
			else if (_timePassed < 0.815f)
				_compPhy->AddForce(MagneticForce(Vector3D{ -1,0,0 }, AISpeed_, dt));
		}
		else if (_timePassed > 2.0f)
		{
			_timePassed = 0;
			_right = !_right;
		}

		// Check if need to chase
		if (!_hitObj && FwMath::Vector3DDistance(_compPhy->_transformComponent->_currentPosition,
			_playerCompTrn->_currentPosition) < _distance)
		{
			_compAI->RunAI("Chase");
		}
	}
	void AIRoaming::Free()
	{
	}
	void AIRoaming::OnCollisionEnter(size_t)
	{
		_timePassed = 1.0f;
		_compPhy->SetVelocity({ 0, _compPhy->_velocity.y ,0 });
	}
	void AIRoaming::OnCollisionExit(size_t)
	{
	}
	void AIRoaming::OnCollisionStay(size_t id)
	{
		GameObjectPool* pool = _compScript->getParentGameObject()->GetParentObjectPool();

		ComponentPhysics& otherComp = (*pool->GetContainerPtr<ComponentPhysics>())[id];
		if (std::find_if(std::begin(TagsToPush_), std::end(TagsToPush_),
			[&](std::string str)
			{
				if (otherComp.isEnabled())
					return (str == otherComp._transformComponent->_tag);
				else
					return false;
			}) != std::end(TagsToPush_))
		{
			if (otherComp._transformComponent->_tag != "player")
			{
				float direction = 1;
				if (otherComp._transformComponent->_currentPosition.x < _compPhy->_transformComponent->_currentPosition.x)
					direction = -1;
				_compPhy->AddForce(MagneticForce(Vector3D{ -direction, 0, 0 }, AISpeed_ * 2.0f + otherComp._velocity.x, 0.001f));
			}
		}
	}
}