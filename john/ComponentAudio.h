/******************************************************************************/
/*!
\file		ComponentAudio.cpp
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
#include <string>
#include "fmod.hpp"
#include "FwAudio.h"
#include <vector>
#include "ComponentTransform.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_AUDIO = "audio";
	class ComponentAudio : public IComponent
	{
	private:
		Channel _channel;
		Sound _sound;
		ComponentTransform* _transform;

	public:
		std::vector<std::string> _audioFileNames;
		std::vector<std::string> _audioNames;

		std::string _currentAudio;
		std::string _defaultAudio;
		float _volume;
		float _min;
		float _max;
		bool _mute;
		bool _loop;
		bool _play;
		bool _3dSound;

		ComponentAudio();
		ComponentAudio(GameObject* parent);
		~ComponentAudio();

		Channel& GetChannel();
		Sound& GetSound();

		void SetSound(std::string fileName);
		void Play(std::string name = "", bool loop = true, bool overwrite = false);

		void SetVolume();

		void MuteSound();
		void UnmuteSound();
		void PauseSound();
		void ResumeSound();
		void StopSound(bool stopLoop = true);

		bool IsPlaying();

		void AddNewAudio();
		void DeleteAudio(size_t index);

		void Toggle3D();
		void SetAudioPosition();
		void SetAudioMinMax();

		ComponentTransform* GetTransform();
		void SetTransform(ComponentTransform* transform);

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}

