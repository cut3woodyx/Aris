#pragma once

#include "IComponent.h"
#include "ComponentTransform.h"
#include "FWMath.h"
#include <list>
namespace FwEngine
{
	constexpr auto STRING_COMPONENT_PHYSICS = "physics";
	typedef FwMath::Vector3D Vector3D;

	class DirectionalForce;
	class RotationalForce;
	class ComponentCollision;

	class ComponentPhysics : public IComponent
	{
	public:
		Vector3D _previousPosition;

		bool _isStatic;
		float _mass;
		//for force
		Vector3D _velocity;
		Vector3D _acceleration;
		std::list<DirectionalForce*> ListOfDirectionalForces;
		std::list<RotationalForce*> ListOfRotationalForces;
		Vector3D _accumulatedForce;

		ComponentTransform* _transformComponent;
		ComponentCollision* _collisionComponent;
		ComponentPhysics();
		ComponentPhysics(GameObject* parent);
		~ComponentPhysics();

		void AddForce(const Vector3D& force);

		void AddForce(DirectionalForce& force);

		void AddForce(RotationalForce& force);


		void SetPosition(Vector3D pos);
		void SetVelocity(Vector3D vec);

		void Init(ParamValueMap& paramValues) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}