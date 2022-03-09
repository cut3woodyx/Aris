#include "AIChase.h"
#include "FWMath.h"

namespace FwEngine
{
	std::vector<std::string> TagsToPush2_
	{
		"player",
		"metal"
	};

	AIChase::AIChase(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase{ "Chase", ai, scr, var }, _compPhy{ nullptr }, 
		_playerCompPhy{ nullptr }, _chasing{ false }, _chaseSpeed{ 0 },
		_chaseDistance{ 0 }, _explodeDistance{ 0 }
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
				_playerCompPhy =
				&_compScript->getParentGameObject()->GetParentObjectPool()->GetContainerPtr<ComponentPhysics>()->at
					(
						it->getParentGameObject()->GetIndex()
					);
			}
		}

		
	}
	AIChase::~AIChase()
	{
	}
	void AIChase::Init()
	{
		if (!_variable.empty())
		{
			std::vector<std::string> tokens = Tokenize(_variable, "#");
			int i = 0;
			if (tokens.size() > i)
				_chaseSpeed = std::stof(tokens[i++]);
			if (tokens.size() > i)
				_chaseDistance = std::stof(tokens[i++]);
			if (tokens.size() > i)
				_explodeDistance = std::stof(tokens[i++]);
		}
	}
	void AIChase::Update(float)
	{
		float distToPlayer = FwMath::Vector3DDistance(_compPhy->_transformComponent->_currentPosition,
			_playerCompPhy->_transformComponent->_currentPosition);

		if (distToPlayer < _chaseDistance)
		{
			// Set velocity here
			_chasing = true;
		}
		else
		{
			_chasing = false;
			//_compPhy->SetVelocity({ 0, _compPhy->_velocity.y ,0 });
			_compAI->RunAI("Roaming");
		}

		if (_chasing)
		{
			if (distToPlayer > _explodeDistance)
			{
				// Move in direction to player
				float direction = 1;
				if (_playerCompPhy->_transformComponent->_currentPosition.x < _compPhy->_transformComponent->_currentPosition.x)
					direction = -1;
				_compPhy->AddForce(MagneticForce(Vector3D{ direction, 0, 0 }, _chaseSpeed, 0.001f));
			}
			else
			{
				//swap
				_compAI->RunAI("Explode");
			}
			
		}
	}
	void AIChase::Free()
	{
	}
	void AIChase::OnCollisionEnter(size_t)
	{
	}
	void AIChase::OnCollisionExit(size_t)
	{
	}
	void AIChase::OnCollisionStay(size_t id)
	{
		GameObjectPool* pool = _compScript->getParentGameObject()->GetParentObjectPool();

		ComponentPhysics& otherComp = (*pool->GetContainerPtr<ComponentPhysics>())[id];
		if (std::find_if(std::begin(TagsToPush2_), std::end(TagsToPush2_),
			[&](std::string str)
			{
				if (otherComp.isEnabled())
					return (str == otherComp._transformComponent->_tag);
				else
					return false;
			}) != std::end(TagsToPush2_))
		{
			if (otherComp._transformComponent->_tag != "player")
			{
				float direction = 1;
				if (_playerCompPhy->_transformComponent->_currentPosition.x < _compPhy->_transformComponent->_currentPosition.x)
					direction = -1;
				_compPhy->AddForce(MagneticForce(Vector3D{ -direction, 0, 0 }, _chaseSpeed * 2.0f + otherComp._velocity.x, 0.001f));
			}
		}
	}
}