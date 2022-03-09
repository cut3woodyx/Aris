/******************************************************************************/
/*!
\file		ComponentEmitter.cpp
\project	Aris
\author 	Primary: Calvin Boey
\par    	email: s.boey\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "ComponentEmitter.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include <cstdlib>

namespace FwEngine
{
	//Main Particle Save Variables
	constexpr auto STRING_EMIITER_SHAPETYPE = "shape_type";
	constexpr auto STRING_DURATION_CYCLE = "time_cycle";
	constexpr auto STRING_RATE = "rate";
	constexpr auto STRING_MAXPARTICLES = "max_particles";

	//Circle/Cone Save Variable
	constexpr auto STRING_RADIUS = "radius";
	constexpr auto STRING_ANGLE = "angle";

	//Size Save Variable
	constexpr auto STRING_RANDOM_SIZE = "random_size";
	constexpr auto STRING_MIN_SIZE = "min_size";
	constexpr auto STRING_MAX_SIZE = "max_size";

	//Speed Save Variable
	constexpr auto STRING_RANDOM_SPEED = "random_speed";
	constexpr auto STRING_MIN_SPEED = "min_speed";
	constexpr auto STRING_MAX_SPEED = "max_speed";

	//Lifespan Save Variable
	constexpr auto STRING_RANDOM_LIFE = "random_life";
	constexpr auto STRING_MIN_LIFE = "min_life";
	constexpr auto STRING_MAX_LIFE = "max_life";

	//Boolean Save Variables
	constexpr auto STRING_PLAY = "play";
	constexpr auto STRING_PREWARM = "prewarm";
	constexpr auto STRING_BURST = "burst";
	constexpr auto STRING_LOOP = "loop";
	constexpr auto STRING_REVERSE = "reverse";
	constexpr auto STRING_FOLLOW = "follow";
	constexpr auto STRING_FADE = "fade";

	//Type Save Variables
	constexpr auto STRING_VELOCITY_OVERTIME = "velocity";
	constexpr auto STRING_SIZE_OVERTIME = "size";
	constexpr auto STRING_SPEED_OVERTIME = "speed";
	constexpr auto STRING_COLOUR_OVERTIME = "colour";

	//Colour Save Variables
	//Randomizer
	constexpr auto STRING_RANDOM_COLOUR = "random_colour";
	constexpr auto STRING_A_RED = "aRed";
	constexpr auto STRING_A_GREEN = "aGreen";
	constexpr auto STRING_A_BLUE = "aBlue";
	constexpr auto STRING_A_ALPHA = "aAlpha";
	constexpr auto STRING_B_RED = "bRed";
	constexpr auto STRING_B_GREEN = "bGreen";
	constexpr auto STRING_B_BLUE = "bBlue";
	constexpr auto STRING_B_ALPHA = "bAlpha";

	//For Colour overtime
	constexpr auto STRING_START_RED = "sRed";
	constexpr auto STRING_START_GREEN = "sGreen";
	constexpr auto STRING_START_BLUE = "sBlue";
	constexpr auto STRING_START_ALPHA = "sAlpha";
	constexpr auto STRING_END_RED = "eRed";
	constexpr auto STRING_END_GREEN = "eGreen";
	constexpr auto STRING_END_BLUE = "eBlue";
	constexpr auto STRING_END_ALPHA = "eAlpha";

	//Velocity Save Variable
	constexpr auto STRING_VELOCITY_X = "velX";
	constexpr auto STRING_VELOCITY_Y = "velY";

	float ComponentEmitter::RandFloat()
	{
		//Always between 0 and 1.0f
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}

	ComponentEmitter::ComponentEmitter() : 
		_particles{}, _alive{}, _numAlive{ 0 }, _compTransform{ nullptr }, _emitterTime{ 0.0f }, _timePassed{ 0.0f }, //Private
		_type{ DEFAULT_SPRITE }, _durationPerCycle{ 5.0f }, _emissionRate{ 1.0f }, _maxParticles{ 100 }, //Main particle data
		_spawnAngle{ 30.0f }, _radius{ 100.0f }, //For cone and circle
		_randomizeSize{ false }, _minParticleSize{ 10 }, _maxParticleSize{ 100 }, //Particle Size
		_randomizeSpeed{ false }, _minParticleSpeed{ 10 }, _maxParticleSpeed{ 100 }, //Particle Speed
		_randomizeLifespan{ false }, _minLifespan{ 1.0f }, _maxLifespan { 5.0f }, //Particle Lifespan
		_play{ false }, _preWarm{ false }, _burst{ false }, _loop{ false }, _reverse{ false }, _follow{ false },//System Booleans
		_velocityOverTime{ false }, _sizeOverTime{ false }, _speedOverTime{ false }, _colourOverTime{ false },  //Type Booleans
		_randomizeColour{ false }, _colourA{ 1.0f,1.0f,1.0f,1.0f }, _colourB{ 1.0f,1.0f,1.0f,1.0f },
		_colourStart{ 1.0f,1.0f,1.0f,1.0f }, _colourEnd{ 1.0f,1.0f,1.0f,1.0f }, _velocity{ 0.0f,0.0f,0.0f } //Data if boolean is true
	{
		//Create the container and set it to this size
		_particles.resize(_maxParticles);
		_alive.resize(_maxParticles);
	}

	ComponentEmitter::ComponentEmitter(GameObject* parent) : IComponent(parent), 
		_particles{}, _alive{}, _numAlive{ 0 }, _compTransform{ nullptr }, _emitterTime{ 0.0f }, _timePassed{ 0.0f }, //Private
		_type{ DEFAULT_SPRITE }, _durationPerCycle{ 5.0f }, _emissionRate{ 1.0f }, _maxParticles{ 100 }, //Main particle data
		_spawnAngle{ 30.0f }, _radius{ 100.0f }, //For cone and circle
		_randomizeSize{ false }, _minParticleSize{ 10 }, _maxParticleSize{ 100 }, //Particle Size
		_randomizeSpeed{ false }, _minParticleSpeed{ 10 }, _maxParticleSpeed{ 100 }, //Particle Speed
		_randomizeLifespan{ false }, _minLifespan{ 1.0f }, _maxLifespan{ 5.0f }, //Particle Lifespan
		_play{ false }, _preWarm{ false }, _burst{ false }, _loop{ false }, _reverse{ false }, _follow{ false }, //System Booleans
		_velocityOverTime{ false }, _sizeOverTime{ false }, _speedOverTime{ false }, _colourOverTime{ false }, //Type Booleans
		_randomizeColour{ false }, _colourA{ 1.0f,1.0f,1.0f,1.0f }, _colourB{ 1.0f,1.0f,1.0f,1.0f }, 
		_colourStart{ 1.0f,1.0f,1.0f,1.0f }, _colourEnd{ 1.0f,1.0f,1.0f,1.0f }, _velocity{ 0.0f,0.0f,0.0f } //Data if boolean is true
	{
		//Create the container and set it to this size
		_particles.resize(_maxParticles);
		_alive.resize(_maxParticles);
	}


	ComponentEmitter::~ComponentEmitter()
	{
		
	}

	ComponentTransform* ComponentEmitter::GetTransform()
	{
		if (!_compTransform)
		{
			_compTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		}

		return _compTransform;
	}

	void ComponentEmitter::SetTransform(ComponentTransform* trans)
	{
		_compTransform = trans;
	}

	bool ComponentEmitter::IsAlive(size_t index)
	{
		return _alive[index];
	}

	void ComponentEmitter::SetAlive(size_t index, bool alive)
	{
		_alive[index] = alive;
	}

	void ComponentEmitter::AddTime(float dt)
	{
		_timePassed += dt;
		_emitterTime += dt;
	}

	void ComponentEmitter::ResetEmitterTime()
	{
		_emitterTime = 0.0f;
	}

	float ComponentEmitter::GetEmitterTime()
	{
		return _emitterTime;
	}

	void ComponentEmitter::ResetTimePassed()
	{
		_timePassed = 0.0f;
	}

	float ComponentEmitter::GetTimePassed()
	{
		return _timePassed;
	}

	void ComponentEmitter::InitParticles()
	{
		if (_numAlive == _maxParticles)
		{
			return;
		}

		//Make sure inside have a transform
		GetTransform();

		switch (_type)
		{
		case DEFAULT_SPRITE:
		{
			for (size_t i = 0; i < _maxParticles; ++i)
			{
				//Randomize spawning location within sprite boundary
				FwMath::Vector3D spawnPos;
				if (!_follow)
				{
					spawnPos = GetTransform()->_currentPosition;
				}
				spawnPos.x += (RandFloat() * _compTransform->_scale.x) - (0.5f * _compTransform->_scale.x);
				spawnPos.y += (RandFloat() * _compTransform->_scale.y) - (0.5f * _compTransform->_scale.y);
				_particles[i]._position = spawnPos;

				//Doesn't move
				_particles[i]._velocity = { 0.0f, 0.0f, 0.0f };

				//Randomize speed
				if (_randomizeSpeed)
				{
					_particles[i]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
				}
				else
				{
					_particles[i]._speed = _maxParticleSpeed;
				}

				//Randomize lifespan
				if (_randomizeLifespan)
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
				}
				else
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = _maxLifespan;
				}

				if (_colourOverTime)
				{
					//Set starting and ending colour
					_particles[i]._colourStart = _colourStart;
					_particles[i]._colourEnd = _colourEnd;
				}
				else if (_randomizeColour)
				{
					//Either A or B
					_particles[i]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
				}
				else
				{
					_particles[i]._colourEnd = _colourB;
				}

				//Randomize size
				if (_randomizeSize)
				{
					_particles[i]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
				}
				else
				{
					_particles[i]._size = _maxParticleSize;
				}

				//Remember to set it alive
				_alive[i] = true;

				//Increase alive count
				++_numAlive;
			}
		}
		break;

		case CIRCLE:
		{
			for (size_t i = 0; i < _maxParticles; ++i)
			{
				//Randomize spawning location within circle boundary
				FwMath::Vector3D spawnPos;
				if (!_follow)
				{
					spawnPos = GetTransform()->_currentPosition;
				}
				float randomAngle = RandFloat() * _spawnAngle;
				spawnPos.x += FwMath::Cosine(randomAngle) * _radius;
				spawnPos.y += FwMath::Sine(randomAngle) * _radius;
				_particles[i]._position = spawnPos;

				//Flys outwards
				_particles[i]._velocity = { FwMath::Cosine(randomAngle), FwMath::Sine(randomAngle), 0.0f };

				//Randomize speed
				if (_randomizeSpeed)
				{
					_particles[i]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
				}
				else
				{
					_particles[i]._speed = _maxParticleSpeed;
				}

				//Randomize lifespan
				if (_randomizeLifespan)
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
				}
				else
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = _maxLifespan;
				}

				if (_colourOverTime)
				{
					//Set starting and ending colour
					_particles[i]._colourStart = _colourStart;
					_particles[i]._colourEnd = _colourEnd;
				}
				else if (_randomizeColour)
				{
					//Either A or B
					_particles[i]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
				}
				else
				{
					_particles[i]._colourEnd = _colourB;
				}

				//Randomize size
				if (_randomizeSize)
				{
					_particles[i]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
				}
				else
				{
					_particles[i]._size = _maxParticleSize;
				}

				//Remember to set it alive
				_alive[i] = true;

				//Increase alive count
				++_numAlive;
			}
		}
		break;

		case CONE:
		{
			for (size_t i = 0; i < _maxParticles; ++i)
			{
				//For rotation of velocity
				FwMath::Matrix4x4 rotateMat(1);
				FwMath::MtxRotation(rotateMat, GetTransform()->_rotation);

				if (_reverse)
				{
					//Randomize direction within cone
					float randomAngle = (RandFloat() * _spawnAngle) - (_spawnAngle * 0.5f);
					_particles[i]._velocity = { FwMath::Cosine(randomAngle), FwMath::Sine(randomAngle), 0.0f };
					FwMath::Vector4D newDirection = rotateMat * FwMath::Vector4D(_particles[i]._velocity);
					_particles[i]._velocity = { newDirection.x, newDirection.y, 0.0f };

					//Base of the cone
					FwMath::Vector3D spawnPos;
					if (!_follow)
					{
						spawnPos = GetTransform()->_currentPosition;
					}
					spawnPos += (_particles[i]._velocity * _radius);
					_particles[i]._position = spawnPos;

					_particles[i]._velocity = -_particles[i]._velocity;
				}
				else
				{
					//Base of the cone
					FwMath::Vector3D spawnPos;
					if (!_follow)
					{
						spawnPos = GetTransform()->_currentPosition;
					}
					_particles[i]._position = spawnPos;

					//Randomize direction within cone
					float randomAngle = (RandFloat() * _spawnAngle) - (_spawnAngle * 0.5f);
					_particles[i]._velocity = { FwMath::Cosine(randomAngle), FwMath::Sine(randomAngle), 0.0f };
					FwMath::Vector4D newDirection = rotateMat * FwMath::Vector4D(_particles[i]._velocity);
					_particles[i]._velocity = { newDirection.x, newDirection.y, 0.0f };
				}

				//Randomize speed
				if (_randomizeSpeed)
				{
					_particles[i]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
				}
				else
				{
					_particles[i]._speed = _maxParticleSpeed;
				}

				//Randomize lifespan
				if (_randomizeLifespan)
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
				}
				else
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = _maxLifespan;
				}

				if (_colourOverTime)
				{
					//Set starting and ending colour
					_particles[i]._colourStart = _colourStart;
					_particles[i]._colourEnd = _colourEnd;
				}
				else if (_randomizeColour)
				{
					//Either A or B
					_particles[i]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
				}
				else
				{
					_particles[i]._colourEnd = _colourB;
				}

				//Randomize size
				if (_randomizeSize)
				{
					_particles[i]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
				}
				else
				{
					_particles[i]._size = _maxParticleSize;
				}

				//Remember to set it alive
				_alive[i] = true;

				//Increase alive count
				++_numAlive;
			}
		}
		break;

		case DIRECTIONAL:
		{
			for (size_t i = 0; i < _maxParticles; ++i)
			{
				//For rotation of velocity
				FwMath::Matrix4x4 rotateMat(1);
				FwMath::MtxRotation(rotateMat, GetTransform()->_rotation);

				//Randomize spawning location within boundary
				FwMath::Vector3D spawnPos;
				if (!_follow)
				{
					spawnPos = GetTransform()->_currentPosition;
				}
				spawnPos.x += (RandFloat() * _compTransform->_scale.x) - (0.5f * _compTransform->_scale.x);
				spawnPos.y += (RandFloat() * _compTransform->_scale.y) - (0.5f * _compTransform->_scale.y);
				_particles[i]._position = spawnPos;

				//Calculate direction with rotation
				_particles[i]._velocity = { 1.0f, 0.0f, 0.0f };
				FwMath::Vector4D newDirection = rotateMat * FwMath::Vector4D(_particles[i]._velocity);
				_particles[i]._velocity = { newDirection.x, newDirection.y, 0.0f };

				//Randomize speed
				if (_randomizeSpeed)
				{
					_particles[i]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
				}
				else
				{
					_particles[i]._speed = _maxParticleSpeed;
				}

				//Randomize lifespan
				if (_randomizeLifespan)
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
				}
				else
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = _maxLifespan;
				}

				if (_colourOverTime)
				{
					//Set starting and ending colour
					_particles[i]._colourStart = _colourStart;
					_particles[i]._colourEnd = _colourEnd;
				}
				else if (_randomizeColour)
				{
					//Either A or B
					_particles[i]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
				}
				else
				{
					_particles[i]._colourEnd = _colourB;
				}

				//Randomize size
				if (_randomizeSize)
				{
					_particles[i]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
				}
				else
				{
					_particles[i]._size = _maxParticleSize;
				}

				//Remember to set it alive
				_alive[i] = true;

				//Increase alive count
				++_numAlive;
			}
		}
		break;

		default:
			break;
		}
	}

	void ComponentEmitter::ResetParticle(size_t index)
	{
		if (_numAlive == _maxParticles)
		{
			return;
		}
		//Make sure inside have a transform
		GetTransform();
		switch (_type)
		{
		case DEFAULT_SPRITE:
		{
			//Randomize spawning location within sprite boundary
			FwMath::Vector3D spawnPos;
			if (!_follow)
			{
				spawnPos = GetTransform()->_currentPosition;
			}
			spawnPos.x += (RandFloat() * _compTransform->_scale.x) - (0.5f * _compTransform->_scale.x);
			spawnPos.y += (RandFloat() * _compTransform->_scale.y) - (0.5f * _compTransform->_scale.y);
			_particles[index]._position = spawnPos;

			//Doesn't move
			_particles[index]._velocity = { 0.0f, 0.0f, 0.0f };

			//Randomize speed
			if (_randomizeSpeed)
			{
				_particles[index]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
			}
			else
			{
				_particles[index]._speed = _maxParticleSpeed;
			}

			//Randomize lifespan
			if (_randomizeLifespan)
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
			}
			else
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = _maxLifespan;
			}

			if (_colourOverTime)
			{
				//Set starting and ending colour
				_particles[index]._colourStart = _colourStart;
				_particles[index]._colourEnd = _colourEnd;
			}
			else if (_randomizeColour)
			{
				//Either A or B
				_particles[index]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
			}
			else
			{
				_particles[index]._colourEnd = _colourB;
			}

			//Randomize size
			if (_randomizeSize)
			{
				_particles[index]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
			}
			else
			{
				_particles[index]._size = _maxParticleSize;
			}

			//Remember to set it alive
			_alive[index] = true;

			//Increase alive count
			++_numAlive;
		}
		break;

		case CIRCLE:
		{
			//Randomize spawning location within circle boundary
			FwMath::Vector3D spawnPos;
			if (!_follow)
			{
				spawnPos = GetTransform()->_currentPosition;
			}
			float randomAngle = RandFloat() * _spawnAngle;
			spawnPos.x += FwMath::Cosine(randomAngle) * _radius * RandFloat();
			spawnPos.y += FwMath::Sine(randomAngle) * _radius * RandFloat();
			_particles[index]._position = spawnPos;

			//Velocity based on the direction of it being spawn
			_particles[index]._velocity = { FwMath::Cosine(randomAngle), FwMath::Sine(randomAngle), 0.0f };
			
			//Randomize speed
			if (_randomizeSpeed)
			{
				_particles[index]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
			}
			else
			{
				_particles[index]._speed = _maxParticleSpeed;
			}

			//Randomize lifespan
			if (!_randomizeLifespan)
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
			}
			else
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = _maxLifespan;
			}

			if (_colourOverTime)
			{
				//Set starting and ending colour
				_particles[index]._colourStart = _colourStart;
				_particles[index]._colourEnd = _colourEnd;
			}
			else if (_randomizeColour)
			{
				//Either A or B
				_particles[index]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
			}
			else
			{
				_particles[index]._colourEnd = _colourB;
			}

			//Randomize size
			if (_randomizeSize)
			{
				_particles[index]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
			}
			else
			{
				_particles[index]._size = _maxParticleSize;
			}

			//Remember to set it alive
			_alive[index] = true;

			//Increase alive count
			++_numAlive;
		}
		break;

		case CONE:
		{
			FwMath::Matrix4x4 rotateMat(1);
			FwMath::MtxRotation(rotateMat, GetTransform()->_rotation);

			if (_reverse)
			{
				//Randomize direction within cone
				float randomAngle = (RandFloat() * _spawnAngle) - (_spawnAngle * 0.5f);
				_particles[index]._velocity = { FwMath::Cosine(randomAngle), FwMath::Sine(randomAngle), 0.0f };
				FwMath::Vector4D newDirection = rotateMat * FwMath::Vector4D(_particles[index]._velocity);
				_particles[index]._velocity = { newDirection.x, newDirection.y, 0.0f };

				//Spawn at end of cone
				FwMath::Vector3D spawnPos;
				if (!_follow)
				{
					spawnPos = GetTransform()->_currentPosition;
				}
				spawnPos += _particles[index]._velocity * _radius;
				_particles[index]._position = spawnPos;

				_particles[index]._velocity = -_particles[index]._velocity;
			}
			else
			{
				//Base of the cone
				FwMath::Vector3D spawnPos;
				if (!_follow)
				{
					spawnPos = GetTransform()->_currentPosition;
				}
				_particles[index]._position = spawnPos;

				//Randomize direction within cone
				float randomAngle = (RandFloat() * _spawnAngle) - (_spawnAngle * 0.5f);
				_particles[index]._velocity = { FwMath::Cosine(randomAngle), FwMath::Sine(randomAngle), 0.0f };
				FwMath::Vector4D newDirection = rotateMat * FwMath::Vector4D(_particles[index]._velocity);
				_particles[index]._velocity = { newDirection.x, newDirection.y, 0.0f };
			}

			//Randomize speed
			if (_randomizeSpeed)
			{
				_particles[index]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
			}
			else
			{
				_particles[index]._speed = _maxParticleSpeed;
			}

			//Randomize lifespan
			if (_randomizeLifespan)
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
			}
			else
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = _maxLifespan;
			}

			if (_colourOverTime)
			{
				//Set starting and ending colour
				_particles[index]._colourStart = _colourStart;
				_particles[index]._colourEnd = _colourEnd;
			}
			else if (_randomizeColour)
			{
				//Either A or B
				_particles[index]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
			}
			else
			{
				_particles[index]._colourEnd = _colourB;
			}

			//Randomize size
			if (_randomizeSize)
			{
				_particles[index]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
			}
			else
			{
				_particles[index]._size = _maxParticleSize;
			}

			//Remember to set it alive
			_alive[index] = true;

			//Increase alive count
			++_numAlive;
		}
		break;

		case DIRECTIONAL:
		{
			FwMath::Matrix4x4 rotateMat(1);
			FwMath::MtxRotation(rotateMat, GetTransform()->_rotation);

			//Randomize spawning location within cone boundary
			FwMath::Vector3D spawnPos;
			if (!_follow)
			{
				spawnPos = GetTransform()->_currentPosition;
			}
			spawnPos.x += (RandFloat() * _compTransform->_scale.x) - (0.5f * _compTransform->_scale.x);
			spawnPos.y += (RandFloat() * _compTransform->_scale.y) - (0.5f * _compTransform->_scale.y);
			_particles[index]._position = spawnPos;

			//Randomize speed
			_particles[index]._velocity = { 1.0f, 0.0f, 0.0f };
			FwMath::Vector4D newDirection = rotateMat * FwMath::Vector4D(_particles[index]._velocity);
			_particles[index]._velocity = { newDirection.x, newDirection.y, 0.0f };

			//Randomize speed
			if (_randomizeSpeed)
			{
				_particles[index]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
			}
			else
			{
				_particles[index]._speed = _maxParticleSpeed;
			}

			//Randomize lifespan
			if (_randomizeLifespan)
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
			}
			else
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = _maxLifespan;
			}

			if (_colourOverTime)
			{
				//Set starting and ending colour
				_particles[index]._colourStart = _colourStart;
				_particles[index]._colourEnd = _colourEnd;
			}
			else if (_randomizeColour)
			{
				//Either A or B
				_particles[index]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
			}
			else
			{
				_particles[index]._colourEnd = _colourB;
			}

			//Randomize size
			if (_randomizeSize)
			{
				_particles[index]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
			}
			else
			{
				_particles[index]._size = _maxParticleSize;
			}

			//Remember to set it alive
			_alive[index] = true;

			//Increase alive count
			++_numAlive;
		}
		break;

		default:
			break;
		}
	}

	void ComponentEmitter::ZeroParticle(size_t index)
	{
		_particles[index]._position = FwMath::Vector3D();
		_particles[index]._velocity = FwMath::Vector3D();
	}

	std::vector<Particle>& ComponentEmitter::GetParticles()
	{
		return _particles;
	}

	void ComponentEmitter::RespawnParticle()
	{
		//Find a particle that is dead and spawn it
		auto iterator = std::find(_alive.begin(), _alive.end(), false);
		if (iterator != _alive.end())
		{
			//Get index
			size_t index = std::distance(_alive.begin(), iterator);
			//Reset the particle values
			ResetParticle(index);
		}
	}

	void ComponentEmitter::UpdateSize()
	{
		_particles.resize(_maxParticles);
		_alive.resize(_maxParticles);
	}

	void ComponentEmitter::IncreaseAlive()
	{
		++_numAlive;
	}

	void ComponentEmitter::DecreaseAlive()
	{
		--_numAlive;
	}

	int ComponentEmitter::GetAlive()
	{
		return _numAlive;
	}

	void ComponentEmitter::Play()
	{
		if (_play)
		{
			return;
		}

		_play = true;
		_emitterTime = 0.0f;
		_timePassed = 0.0f;

		if (_preWarm)
		{
			InitParticles();
		}
	}

	void ComponentEmitter::Stop()
	{
		_play = false;
		_emitterTime = 0.0f;
		_timePassed = 0.0f;

		for (size_t i = 0; i < _maxParticles; ++i)
		{
			if (_alive[i])
			{
				//Decrease alive count
				--_numAlive;
				ZeroParticle(i);
			}
			_alive[i] = false;
		}
	}

	void ComponentEmitter::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		//Main Particle Save Variables
		_type = (EmitterShapeType)std::stoi(GetFirst(paramValues, STRING_EMIITER_SHAPETYPE));
		_durationPerCycle = std::stof(GetFirst(paramValues, STRING_DURATION_CYCLE));
		_emissionRate = std::stof(GetFirst(paramValues, STRING_RATE));
		_maxParticles = std::stoi(GetFirst(paramValues, STRING_MAXPARTICLES));

		//Circle/Cone Save Variable
		_radius = std::stof(GetFirst(paramValues, STRING_RADIUS));
		_spawnAngle = std::stof(GetFirst(paramValues, STRING_ANGLE));

		//Size Save Variable
		_randomizeSize = std::stoi(GetFirst(paramValues, STRING_RANDOM_SIZE));
		_minParticleSize = std::stof(GetFirst(paramValues, STRING_MIN_SIZE));
		_maxParticleSize = std::stof(GetFirst(paramValues, STRING_MAX_SIZE));

		//Speed Save Variable
		_randomizeSpeed = std::stoi(GetFirst(paramValues, STRING_RANDOM_SPEED));
		_minParticleSpeed = std::stof(GetFirst(paramValues, STRING_MIN_SPEED));
		_maxParticleSpeed = std::stof(GetFirst(paramValues, STRING_MAX_SPEED));

		//Lifespan Save Variable
		_randomizeLifespan = std::stoi(GetFirst(paramValues, STRING_RANDOM_LIFE));
		_minLifespan = std::stof(GetFirst(paramValues, STRING_MIN_LIFE));
		_maxLifespan = std::stof(GetFirst(paramValues, STRING_MAX_LIFE));

		//Boolean Save Variables
		_play = std::stoi(GetFirst(paramValues, STRING_PLAY));
		_preWarm = std::stoi(GetFirst(paramValues, STRING_PREWARM));
		_loop = std::stoi(GetFirst(paramValues, STRING_LOOP));
		_burst = std::stoi(GetFirst(paramValues, STRING_BURST));
		_reverse = std::stoi(GetFirst(paramValues, STRING_REVERSE));
		_follow = std::stoi(GetFirst(paramValues, STRING_FOLLOW));
		_fade = std::stoi(GetFirst(paramValues, STRING_FADE));

		_velocityOverTime = std::stoi(GetFirst(paramValues, STRING_VELOCITY_OVERTIME));
		_sizeOverTime = std::stoi(GetFirst(paramValues, STRING_SIZE_OVERTIME));
		_speedOverTime = std::stoi(GetFirst(paramValues, STRING_SPEED_OVERTIME));
		_colourOverTime = std::stoi(GetFirst(paramValues, STRING_COLOUR_OVERTIME));

		_randomizeColour = std::stoi(GetFirst(paramValues, STRING_RANDOM_COLOUR));
		//Colour Save Variables
		float red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;
		//Checks if colour saves exist, if not use default
		if (paramValues.find(STRING_A_RED) != paramValues.end())
		{
			red = std::stof(GetFirst(paramValues, STRING_A_RED));
		}
		if (paramValues.find(STRING_A_GREEN) != paramValues.end())
		{
			green = std::stof(GetFirst(paramValues, STRING_A_GREEN));
		}
		if (paramValues.find(STRING_A_BLUE) != paramValues.end())
		{
			blue = std::stof(GetFirst(paramValues, STRING_A_BLUE));
		}
		if (paramValues.find(STRING_A_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_A_ALPHA));
		}
		_colourA= FwMath::Vector4D(red, green, blue, alpha);

		//Checks if colour saves exist, if not use default
		if (paramValues.find(STRING_B_RED) != paramValues.end())
		{
			red = std::stof(GetFirst(paramValues, STRING_B_RED));
		}
		if (paramValues.find(STRING_B_GREEN) != paramValues.end())
		{
			green = std::stof(GetFirst(paramValues, STRING_B_GREEN));
		}
		if (paramValues.find(STRING_B_BLUE) != paramValues.end())
		{
			blue = std::stof(GetFirst(paramValues, STRING_B_BLUE));
		}
		if (paramValues.find(STRING_B_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_B_ALPHA));
		}
		_colourB = FwMath::Vector4D(red, green, blue, alpha);

		//Checks if colour saves exist, if not use default
		if (paramValues.find(STRING_START_RED) != paramValues.end())
		{
			red = std::stof(GetFirst(paramValues, STRING_START_RED));
		}
		if (paramValues.find(STRING_START_GREEN) != paramValues.end())
		{
			green = std::stof(GetFirst(paramValues, STRING_START_GREEN));
		}
		if (paramValues.find(STRING_START_BLUE) != paramValues.end())
		{
			blue = std::stof(GetFirst(paramValues, STRING_START_BLUE));
		}
		if (paramValues.find(STRING_START_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_START_ALPHA));
		}
		_colourStart = FwMath::Vector4D(red, green, blue, alpha);

		//Checks if colour saves exist, if not use default
		red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;
		if (paramValues.find(STRING_END_RED) != paramValues.end())
		{
			red = std::stof(GetFirst(paramValues, STRING_END_RED));
		}
		if (paramValues.find(STRING_END_GREEN) != paramValues.end())
		{
			green = std::stof(GetFirst(paramValues, STRING_END_GREEN));
		}
		if (paramValues.find(STRING_END_BLUE) != paramValues.end())
		{
			blue = std::stof(GetFirst(paramValues, STRING_END_BLUE));
		}
		if (paramValues.find(STRING_END_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_END_ALPHA));
		}
		_colourEnd = FwMath::Vector4D(red, green, blue, alpha);

		//Velocity Save Variable
		_velocity.x = std::stof(GetFirst(paramValues, STRING_VELOCITY_X));
		_velocity.y = std::stof(GetFirst(paramValues, STRING_VELOCITY_Y));
		_velocity.z = 0.0f;

		//Erase the previous instance of the particles and reinitialize everything
		_particles.clear();
		_particles.resize(_maxParticles, Particle());

		_alive.clear();
		_alive.resize(_maxParticles, false);

		//If required to prewarm the system and it is playing init all particles
		if (_preWarm && _play)
		{
			InitParticles();
		}

		_compTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
	}

	void ComponentEmitter::Clone(IComponent& source)
	{
		_isEnabled = source.isEnabled();

		ComponentEmitter& emitter = reinterpret_cast<ComponentEmitter&>(source);

		_type = emitter._type;
		_durationPerCycle = emitter._durationPerCycle;
		_emissionRate = emitter._emissionRate;
		_maxParticles = emitter._maxParticles;

		_radius = emitter._radius;
		_spawnAngle = emitter._spawnAngle;

		_randomizeSize = emitter._randomizeSize;
		_minParticleSize = emitter._minParticleSize;
		_maxParticleSize = emitter._maxParticleSize;

		_randomizeSpeed = emitter._randomizeSpeed;
		_minParticleSpeed = emitter._minParticleSpeed;
		_maxParticleSpeed = emitter._maxParticleSpeed;

		_randomizeLifespan = emitter._randomizeLifespan;
		_minLifespan = emitter._minLifespan;
		_maxLifespan = emitter._maxLifespan;

		_play = emitter._play;
		_preWarm = emitter._preWarm;
		_burst = emitter._burst;
		_loop = emitter._loop;
		_reverse = emitter._reverse;
		_follow = emitter._follow;
		_fade = emitter._fade;

		_velocityOverTime = emitter._velocityOverTime;
		_sizeOverTime = emitter._sizeOverTime;
		_speedOverTime = emitter._speedOverTime;
		_colourOverTime = emitter._colourOverTime;

		_randomizeColour = emitter._randomizeColour;
		_colourA = emitter._colourA;
		_colourB = emitter._colourB;

		_colourStart = emitter._colourStart;
		_colourEnd = emitter._colourEnd;

		_velocity = emitter._velocity;

		//Private
		_emitterTime = emitter._emitterTime;
		_timePassed = emitter._timePassed;
		_particles = emitter._particles;
		_alive = emitter._alive;
		_numAlive = emitter._numAlive;

		_compTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
	}

	void ComponentEmitter::Update(float)
	{
	}

	void ComponentEmitter::Free()
	{
		_play = false;
		_particles.clear();
		_particles.resize(_maxParticles);
		_particles.assign(_maxParticles, Particle());
	}

	void ComponentEmitter::Destroy()
	{
	}

	std::pair<std::string, ParamValueMap> ComponentEmitter::GetParams()
	{
		ParamValueMap params;

		params.emplace(STRING_EMIITER_SHAPETYPE, std::to_string(_type));
		params.emplace(STRING_RATE, std::to_string(_emissionRate));
		params.emplace(STRING_DURATION_CYCLE, std::to_string(_durationPerCycle));
		params.emplace(STRING_MAXPARTICLES, std::to_string(_maxParticles));

		params.emplace(STRING_RADIUS, std::to_string(_radius));
		params.emplace(STRING_ANGLE, std::to_string(_spawnAngle));

		params.emplace(STRING_RANDOM_SIZE, std::to_string(_randomizeSize));
		params.emplace(STRING_MIN_SIZE, std::to_string(_minParticleSize));
		params.emplace(STRING_MAX_SIZE, std::to_string(_maxParticleSize));

		params.emplace(STRING_RANDOM_SPEED, std::to_string(_randomizeSpeed));
		params.emplace(STRING_MIN_SPEED, std::to_string(_minParticleSpeed));
		params.emplace(STRING_MAX_SPEED, std::to_string(_maxParticleSpeed));

		params.emplace(STRING_RANDOM_LIFE, std::to_string(_randomizeLifespan));
		params.emplace(STRING_MIN_LIFE, std::to_string(_minLifespan));
		params.emplace(STRING_MAX_LIFE, std::to_string(_maxLifespan));

		params.emplace(STRING_PLAY, std::to_string((int)_play));
		params.emplace(STRING_PREWARM, std::to_string((int)_preWarm));
		params.emplace(STRING_LOOP, std::to_string((int)_loop));
		params.emplace(STRING_BURST, std::to_string((int)_burst));
		params.emplace(STRING_REVERSE, std::to_string((int)_reverse));
		params.emplace(STRING_FOLLOW, std::to_string((int)_follow));
		params.emplace(STRING_FADE, std::to_string((int)_fade));

		params.emplace(STRING_VELOCITY_OVERTIME, std::to_string(_velocityOverTime));
		params.emplace(STRING_SIZE_OVERTIME, std::to_string(_sizeOverTime));
		params.emplace(STRING_SPEED_OVERTIME, std::to_string(_speedOverTime));
		params.emplace(STRING_COLOUR_OVERTIME, std::to_string(_colourOverTime));

		params.emplace(STRING_RANDOM_COLOUR, std::to_string((int)_randomizeColour));
		params.emplace(STRING_A_RED, std::to_string(_colourA.x));
		params.emplace(STRING_A_GREEN, std::to_string(_colourA.y));
		params.emplace(STRING_A_BLUE, std::to_string(_colourA.z));
		params.emplace(STRING_A_ALPHA, std::to_string(_colourA.w));
		params.emplace(STRING_B_RED, std::to_string(_colourB.x));
		params.emplace(STRING_B_GREEN, std::to_string(_colourB.y));
		params.emplace(STRING_B_BLUE, std::to_string(_colourB.z));
		params.emplace(STRING_B_ALPHA, std::to_string(_colourB.w));

		params.emplace(STRING_START_RED, std::to_string(_colourStart.x));
		params.emplace(STRING_START_GREEN, std::to_string(_colourStart.y));
		params.emplace(STRING_START_BLUE, std::to_string(_colourStart.z));
		params.emplace(STRING_START_ALPHA, std::to_string(_colourStart.w));
		params.emplace(STRING_END_RED, std::to_string(_colourEnd.x));
		params.emplace(STRING_END_GREEN, std::to_string(_colourEnd.y));
		params.emplace(STRING_END_BLUE, std::to_string(_colourEnd.z));
		params.emplace(STRING_END_ALPHA, std::to_string(_colourEnd.w));

		params.emplace(STRING_VELOCITY_X, std::to_string(_velocity.x));
		params.emplace(STRING_VELOCITY_Y, std::to_string(_velocity.y));

		return { STRING_COMPONENT_EMITTER, params };
	}
}
