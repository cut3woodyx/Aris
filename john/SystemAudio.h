/******************************************************************************/
/*!
\file		SystemAudio.h
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

#include "ISystem.h"
#include "fmod.hpp"
#include "ResourceManager.h"
#include "FwAudio.h"

namespace FwEngine
{
	class SystemAudio : public ISystem
	{
	private:
		System _audioSystem;
		FwMath::Vector3D _audioPosition;

	public:
		SystemAudio();
		~SystemAudio();

		System& GetSystem();
		void UpdateAudio(ComponentAudio* audio);

		void Init() override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;
	};

	extern SystemAudio* AUDIO;
}

