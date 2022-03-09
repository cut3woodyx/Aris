#pragma once
#include "FWMath.h"

struct Particle
{
	FwMath::Vector3D _position;
	FwMath::Vector3D _velocity;
	FwMath::Vector4D _colourStart;
	FwMath::Vector4D _colourEnd;
	float _timeAlive;
	float _lifespan;
	float _size;
	float _speed;

	Particle();
	~Particle() = default;
};