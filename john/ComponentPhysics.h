/******************************************************************************/
/*!
\file		ComponentPhysics.h
\project	Aris
\author 	Primary: Wang YiDi
\par    	email: w.yidi\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include "IComponent.h"
#include "ComponentTransform.h"
#include "FWMath.h"
#include <list>
#include <map>
namespace FwEngine
{
	constexpr auto STRING_COMPONENT_PHYSICS = "physics";
	typedef FwMath::Vector3D Vector3D;

	class DirectionalForce;
	class RotationalForce;
	class MagneticForce;
	class ComponentCollision;

	class ComponentPhysics : public IComponent
	{
	public:
		Vector3D _previousPosition;

		bool _isStatic;
		float _mass;
		//for force
		float coefficient_of_friction_Static;
		float coefficient_of_friction_Kinetic;
		Vector3D _velocity;
		Vector3D _preVelocity;
		Vector3D _acceleration;
		
		std::list<DirectionalForce*> ListOfDirectionalForces;
		std::list<RotationalForce*> ListOfRotationalForces;
		std::list<MagneticForce> ListOfMagneticForces;
		Vector3D _accumulatedForce;

		ComponentTransform* _transformComponent;
		ComponentCollision* _collisionComponent;
		ComponentPhysics();
		ComponentPhysics(GameObject* parent);
		~ComponentPhysics();

		void AddForce(const Vector3D& force);

		void AddForce(DirectionalForce& force);

		void AddForce(RotationalForce& force);

		void AddForce(MagneticForce force);


		void SetPosition(Vector3D pos);
		void SetVelocity(Vector3D vec);

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}