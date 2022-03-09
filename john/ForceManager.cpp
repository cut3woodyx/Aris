#include "ForceManager.h"
#include <iostream>
//***************************************************************Functions for DirectionalForce**********************************************************//

namespace FwEngine
{
	DirectionalForce::DirectionalForce() :_direction{ 0 }, _magnitude{ 0 }, _lifetime{ 0 }, _age{ 0 }, _is_active{ false }
	{
	}

	DirectionalForce::DirectionalForce(FwMath::Vector3D direction, float magnitude, float lifetime)
		: _direction{ direction },
		_magnitude{ magnitude },
		_lifetime{ lifetime },
		_age{ 0 },
		_is_active{ false }
	{
	}

	FwMath::Vector3D DirectionalForce::ReturnForce() const
	{
		return _direction * _magnitude;
	}

	FwMath::Vector3D DirectionalForce::ApplyForce(float dt)
	{
		//if it is an active force
		if (_is_active)
		{
			//if lifetime runs out
			if (_lifetime <= _age)
			{
				//returns a zero vector
				DeActivate();
				return FwMath::Vector3D{ 0 };
			}
			_age += dt;
			return _direction * _magnitude;
		}
		return FwMath::Vector3D{ 0 };
	}

	void DirectionalForce::DeActivate()
	{
		_age = 0;
		_is_active = false;
	}

	void DirectionalForce::SetActivate()
	{
		_is_active = true;
		_age = 0;
	}

	float DirectionalForce::ValidateAge() const
	{
		return _age;
	}

	void DirectionalForce::SetLifeTime(float lifetime)
	{
		_lifetime = lifetime;
	}

	bool DirectionalForce::checkValidity() const
	{
		return _is_active;
	}

	DirectionalForce DirectionalForce::operator+(const DirectionalForce& rhs)
	{
		return DirectionalForce{
			_direction + rhs._direction,
			_magnitude + rhs._magnitude };

	}

	DirectionalForce DirectionalForce::operator-(const DirectionalForce& rhs)
	{
		return DirectionalForce{
			_direction - rhs._direction,
			_magnitude - rhs._magnitude };
	}

	DirectionalForce& DirectionalForce::operator=(const DirectionalForce& rhs)
	{
		_direction = rhs._direction;
		_magnitude = rhs._magnitude;
		_lifetime = rhs._lifetime;
		_age = rhs._age;
		_is_active = rhs._is_active;

		return *this;
	}

	DirectionalForce& DirectionalForce::operator+=(const DirectionalForce& rhs)
	{
		_magnitude += rhs._magnitude;
		_direction += rhs._direction;

		return *this;
	}

	DirectionalForce& DirectionalForce::operator-=(const DirectionalForce& rhs)
	{
		_magnitude -= rhs._magnitude;
		_direction -= rhs._direction;

		return *this;
	}

	////////////////////////////////////////////////Roatational Force/////////////////////////////////////////////////////////

	RotationalForce::RotationalForce():_centerPoint{ 0 }, _magnitude{ 0 }, _lifetime{ 0 }, _age{ 0 }, _is_active{ false }
	{
	}
	RotationalForce::RotationalForce(float magnitude, float lifetime, const FwMath::Vector3D& centerPoint)
		: _centerPoint{ centerPoint }, _magnitude{ magnitude }, _lifetime{ lifetime }, _age{ 0 }, _is_active{ true }
	{

	}

	bool RotationalForce::ApplyForce(ComponentCollision& obj, float dt)
	{
		if (_is_active)
		{
			_lifetime -= dt;
			_age += dt;
			//if lifetime runs out
			if (-1 < _lifetime && _lifetime < 0)
			{
				//returns a zero vector
				RemoveForce();
				return false;
			}

			//move all points according to point of rotation
			FwMath::Vector3D centerVertexA = obj._vertexA  + _centerPoint;
			FwMath::Vector3D centerVertexB = obj._vertexB  + _centerPoint;
			FwMath::Vector3D centerVertexC = obj._vertexC  + _centerPoint;
			FwMath::Vector3D centerVertexD = obj._vertexD  + _centerPoint;

			//formming rotation matrix
			FwMath::Matrix3x3 RotationFunc;
			FwMath::Mtx33RotDeg(RotationFunc, _magnitude * dt);
			
			//rotate here
			obj._vertexA = RotationFunc * centerVertexA;
			obj._vertexB = RotationFunc * centerVertexB;
			obj._vertexC = RotationFunc * centerVertexC;
			obj._vertexD = RotationFunc * centerVertexD;

			return true;
		}
		return false;
	}

	void RotationalForce::RemoveForce()
	{
		_is_active = false;
	}

	void RotationalForce::SetActivateForce(bool set)
	{
		_is_active = set;
	}

	float RotationalForce::ValidateAge() const
	{
		return _age;
	}

	void RotationalForce::SetLifeTime(float lifetime)
	{
		_lifetime = lifetime;
	}

	bool RotationalForce::checkValidity() const
	{
		return _is_active;
	}
}