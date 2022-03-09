#pragma once
#include "Vector.h"
#include "ComponentCollision.h"

namespace FwEngine
{
	class DirectionalForce
	{
		FwMath::Vector3D _direction;
		float _magnitude;
		float _lifetime;
		float _age;
		bool _is_active;
	public:
		DirectionalForce();
		//create a force with direction and magnitude and a optional lifetime, if lifetime is not set the force will be ethernal
		DirectionalForce(FwMath::Vector3D direction, float magnitude = 1, float lifetime= -2);
		//set force with a direction and magnitude
		FwMath::Vector3D ReturnForce() const;
		FwMath::Vector3D ApplyForce(float dt);
		void DeActivate();
		void SetActivate() ;
		float ValidateAge() const ;
		void SetLifeTime(float lifetime) ;
		bool checkValidity() const;


		DirectionalForce operator+(const DirectionalForce& rhs);
		DirectionalForce operator-(const DirectionalForce& rhs);
		DirectionalForce& operator=(const DirectionalForce& rhs);
		DirectionalForce& operator+=(const DirectionalForce& rhs);
		DirectionalForce& operator-=(const DirectionalForce& rhs);

	};

	class RotationalForce
	{
		FwMath::Vector3D _centerPoint;
		float _magnitude;
		float _lifetime;
		float _age;
		bool _is_active;
	public:
		RotationalForce();
		RotationalForce(float magnitude, float lifetime, const FwMath::Vector3D& centerPoint = FwMath::Vector3D{0});
		
		//apply rotation to an object, it will return false if force is not active
		bool ApplyForce(ComponentCollision& obj,float dt);


		void RemoveForce();
		void SetActivateForce(bool set);
		float ValidateAge() const;
		void SetLifeTime(float lifetime);
		bool checkValidity() const;
	};

}