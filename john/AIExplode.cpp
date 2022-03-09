#include "AIExplode.h"
#include "SystemManager.h"

namespace FwEngine
{
	AIExplode::AIExplode(AIScript* ai, ComponentScript* scr, std::string var)
		: AIBase("Explode", ai, scr, var), _compPhy{ nullptr }, _compRenderer{ nullptr },
		_playerCompPhy{ nullptr }, _playerCompCol{ nullptr }, _explodeDistance{ 0 },
		_toExplode{ false }, _timeToExplode{ 0 }, _timePassed{ 0 }, _knockBackSpeed{ 0 }, 
		_hitPlayer{ false }
	{
		if (_compScript)
		{
			_compPhy = &_compScript->getComponent<ComponentPhysics>();

			_compRenderer = &_compScript->getComponent<ComponentRenderer>();

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

				_playerCompCol =
					&_compScript->getParentGameObject()->GetParentObjectPool()->GetContainerPtr<ComponentCollision>()->at
					(
						it->getParentGameObject()->GetIndex()
					);
			}
		}
	}
	AIExplode::~AIExplode()
	{
	}
	void AIExplode::Init()
	{
		if (!_variable.empty())
		{
			std::vector<std::string> tokens = Tokenize(_variable, "#");
			int i = 0;
			if (tokens.size() > i)
				_explodeDistance = std::stof(tokens[i++]);
			if (tokens.size() > i)
				_timeToExplode = std::stof(tokens[i++]);
			if (tokens.size() > i)
				_knockBackSpeed = std::stof(tokens[i++]);
		}
		_hitPlayer = false;
	}
	void AIExplode::Update(float dt)
	{
		// Explode 
		_timePassed += dt;
		if (_timePassed <= _timeToExplode)
		{
			float ratio = (_timePassed / _timeToExplode);
			// Get ready to explode // Turns red over time
			_compRenderer->_colour.g = 1.0f - ratio;
			_compRenderer->_colour.b = 1.0f - ratio;
		}
		else
		{
			float timeScale = 0.25f;
			if (_timePassed <= _timeToExplode + timeScale)
			{
				float tP = ((_timePassed - _timeToExplode) / timeScale);
				Vector3D& scale = _compPhy->_transformComponent->_scale;
				
				
				float xS = _playerCompCol->_vertexB.x - _playerCompCol->_vertexA.x;
				float yS = _playerCompCol->_vertexB.y - _playerCompCol->_vertexA.y;

				scale += Vector3D{ tP * (_explodeDistance) * 0.55f,
					tP * (_explodeDistance) * 0.55f * xS/yS, 0 };
			}
			else
			{
				_compPhy->getParentGameObject()->disable();
			}
		}
	}
	void AIExplode::Free()
	{
	}
	void AIExplode::OnCollisionEnter(size_t)
	{
	}
	void AIExplode::OnCollisionExit(size_t)
	{
	}
	void AIExplode::OnCollisionStay(size_t id)
	{
		GameObjectPool* pool = _compScript->getParentGameObject()->GetParentObjectPool();

		ComponentPhysics& otherComp = (*pool->GetContainerPtr<ComponentPhysics>())[id];

		if (_timePassed > _timeToExplode)
		{
			//ComponentCollision& col = (*pool->GetContainerPtr<ComponentCollision>())[id];
			//char check = col.Collision_Check_List.find("Door")->second;


			if (std::find_if(std::begin(TagsToPush_), std::end(TagsToPush_),
				[&](std::string str)
				{
					if (otherComp.isEnabled())
						return (str == otherComp._transformComponent->_tag);
					else
						return false;
				}) != std::end(TagsToPush_))
			{
				Vector3D direction, toCheck;
				toCheck = otherComp._transformComponent->_currentPosition - _compPhy->_transformComponent->_currentPosition;
				FwMath::Vector3DNormalize(direction, toCheck);

				otherComp.AddForce(MagneticForce(direction, _knockBackSpeed, 0.5f));

				if (otherComp._transformComponent->_tag == "player" && !_hitPlayer)
				{
					_hitPlayer = true;
					--_compScript->getSystemManager()._playerMgr.health;
				}
			}
		}
		else
		{
			if (std::find_if(std::begin(TagsToPush_), std::end(TagsToPush_),
				[&](std::string str)
				{
					if (otherComp.isEnabled())
						return (str == otherComp._transformComponent->_tag);
					else
						return false;
				}) != std::end(TagsToPush_))
			{
				float direction = 1;
				if (otherComp._transformComponent->_currentPosition.x < _compPhy->_transformComponent->_currentPosition.x)
					direction = -1;
				_compPhy->AddForce(MagneticForce(Vector3D{ -direction, 0, 0 }, otherComp._velocity.x * 100.0f, 0.01f));
			}
		}
	}
}
