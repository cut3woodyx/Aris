/******************************************************************************/
/*!
\file		ComponentEmitter.h
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
#pragma once

#include "IComponent.h"
#include <map>
#include <vector>
#include "Particle.h"
#include "ComponentTransform.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_EMITTER = "emitter";

	enum EmitterShapeType
	{
		DEFAULT_SPRITE, //Just spawns and does not move around the player
		CIRCLE, //Spawns within a radius range and moves outwards
		CONE, //Spawns within the cone at a set angle and moves at the direction of the cone base
		DIRECTIONAL, //Spawns like a sprite, moves in the same direction
		TOTAL_SHAPE = 4
	};

	class ComponentEmitter : public IComponent
	{
	private:
		std::vector<Particle> _particles;
		std::vector<bool> _alive;
		int _numAlive;
		ComponentTransform* _compTransform;
		float _emitterTime;
		float _timePassed;

		float RandFloat();

	public:
		EmitterShapeType _type;
		float _durationPerCycle;
		float _emissionRate;
		int _maxParticles;

		float _spawnAngle;
		float _radius;

		bool _randomizeSize;
		float _minParticleSize;
		float _maxParticleSize;

		bool _randomizeSpeed;
		float _minParticleSpeed;
		float _maxParticleSpeed;

		bool _randomizeLifespan;
		float _minLifespan;
		float _maxLifespan;

		bool _play;
		bool _preWarm;
		bool _burst;
		bool _loop;
		bool _reverse;
		bool _follow;
		bool _fade;

		bool _velocityOverTime;
		bool _sizeOverTime;
		bool _speedOverTime;
		bool _colourOverTime;

		bool _randomizeColour;
		FwMath::Vector4D _colourA;
		FwMath::Vector4D _colourB;

		FwMath::Vector4D _colourStart;
		FwMath::Vector4D _colourEnd;

		FwMath::Vector3D _velocity;

		ComponentEmitter();
		ComponentEmitter(GameObject* parent);
		~ComponentEmitter();

		ComponentTransform* GetTransform();
		void SetTransform(ComponentTransform* trans);

		bool IsAlive(size_t index);
		void SetAlive(size_t index, bool  alive);

		void AddTime(float dt);
		void ResetEmitterTime();
		float GetEmitterTime();
		void ResetTimePassed();
		float GetTimePassed();

		void InitParticles();
		void ResetParticle(size_t index);
		void ZeroParticle(size_t index);
		std::vector<Particle>& GetParticles();
		void RespawnParticle();

		void UpdateSize();
		void IncreaseAlive();
		void DecreaseAlive();
		int GetAlive();

		void Play();
		void Stop();

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& source) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}

#pragma once
