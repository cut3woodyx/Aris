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
#include "ComponentAudio.h"
#include "ResourceManager.h"
#include "SystemAudio.h"

constexpr auto STRING_AUDIO_FILE = "audio_file";
constexpr auto STRING_AUDIO_NAME = "audio_name";
constexpr auto STRING_VOLUME = "volume";
constexpr auto STRING_MINIMUM = "min";
constexpr auto STRING_MAXIMUM = "max";
constexpr auto STRING_MUTE = "mute";
constexpr auto STRING_DEFAULT = "default";
constexpr auto STRING_3DSOUND = "3d";

namespace FwEngine
{
	ComponentAudio::ComponentAudio() : _channel{ nullptr }, _sound{ nullptr }, _transform{ nullptr },
		_audioFileNames{}, _audioNames{}, _currentAudio{}, _defaultAudio{}, _mute{ false }, _volume{ 1.0f }, _min{ 0.5f }, _max{ 10000.0f }, _loop{ false }, _play{ false },
		_3dSound{ false }
	{
	}

	ComponentAudio::ComponentAudio(GameObject* parent) : IComponent(parent), _channel{ nullptr }, _sound{ nullptr }, _transform{ nullptr },
		_audioFileNames{}, _audioNames{}, _currentAudio{}, _defaultAudio{}, _mute{ false }, _volume{ 1.0f }, _min{ 0.5f }, _max{ 10000.0f }, _loop{ false }, _play{ false },
		_3dSound{ false }
	{
	}

	ComponentAudio::~ComponentAudio()
	{
	}

	Channel& ComponentAudio::GetChannel()
	{
		return _channel;
	}

	Sound& ComponentAudio::GetSound()
	{
		return _sound;
	}

	void ComponentAudio::SetSound(std::string fileName)
	{
		try
		{
			_sound = RESOURCE->GetSound(fileName, AUDIO->GetSystem());
		}
		catch (...)
		{
			_sound = nullptr;
			return;
		}
	}

	void ComponentAudio::Play(std::string name, bool loop, bool overwrite)
	{
		//Check if the given string is empty
		if (name.empty())
		{
			//Check if there is a default audio playing
			if (!_defaultAudio.empty())
			{
				name = _defaultAudio;
			}
			else
			{
				return;
			}
		}

		//If choose not to overwrite check if current audio is playing the same audio requested
		if (!overwrite)
		{
			if (name == _currentAudio && IsPlaying())
			{
				return;
			}
		}

		int index;
		auto it = std::find(_audioNames.begin(), _audioNames.end(), name);

		try
		{
			CDEBUG_ASSERT(it != _audioNames.end(), std::string(name).append("not found.").c_str());
		}
		catch (...)
		{
			return;
		}

		index = (int)std::distance(_audioNames.begin(), it);
		SetSound(_audioFileNames[index]);

		if (_sound == nullptr)
			return;

		//If there is something playing, stop it and empty out the current string
		if (IsPlaying())
		{
			_currentAudio = "";
			StopAudio(_channel);
		}

		if (loop)
		{
			EnableLoop(_sound, _channel);
		}
		else
		{
			DisableLoop(_sound, _channel);
		}

		_currentAudio = name;
		_loop = loop;
		_play = true;

		//3D mode and 2D mode
		if (_3dSound)
		{
			//Set the sound ptr into 3D mode
			//Set the minimum and maximum distance the sound will start to fade away
			SetMode(_sound, FMOD_3D);
			Set3DMinMax(_sound, _min, _max);
			PlayAudio(AUDIO->GetSystem(), _sound, _channel);

			//Remember to set audio poisition
			SetAudioPosition();
			ResumeAudio(_channel);
		}
		else
		{
			SetMode(_sound, FMOD_2D);
			PlayAudio(AUDIO->GetSystem(), _sound, _channel);
			ResumeAudio(_channel);
		}
	}

	void ComponentAudio::SetVolume()
	{
		::FwEngine::SetVolume(_channel, _volume);
	}

	void ComponentAudio::MuteSound()
	{
		::FwEngine::MuteAudio(_channel);
		_mute = true;
	}

	void ComponentAudio::UnmuteSound()
	{
		::FwEngine::UnmuteAudio(_channel);
		_mute = false;
	}

	void ComponentAudio::PauseSound()
	{
		::FwEngine::PauseAudio(_channel);
	}

	void ComponentAudio::ResumeSound()
	{
		::FwEngine::ResumeAudio(_channel);
	}

	void ComponentAudio::StopSound(bool stopLoop)
	{
		if (!stopLoop)
		{
			DisableLoop(_sound, _channel);
			_loop = false;
		}
		else
		{
			StopAudio(_channel);
		}
	}

	bool ComponentAudio::IsPlaying()
	{
		bool temp;
		::FwEngine::IsPlayingAudio(_channel, temp);
		return temp;
	}

	void ComponentAudio::AddNewAudio()
	{
		_audioNames.push_back("");
		_audioFileNames.push_back("");
	}

	void ComponentAudio::DeleteAudio(size_t index)
	{
		auto itName = _audioNames.begin() + index;
		auto itFilename = _audioFileNames.begin() + index;
		_audioNames.erase(itName);
		_audioFileNames.erase(itFilename);
	}

	void ComponentAudio::Toggle3D()
	{
		//Previously in 2D mode, just changed into 3D and it is playing
		if (_3dSound)
		{
			if (_play)
			{
				//Stop the original 2D Sound and change sound into 3D mode
				StopAudio(_channel);
				SetMode(_sound, FMOD_3D);
				Set3DMinMax(_sound, _min, _max);
				PlayAudio(AUDIO->GetSystem(), _sound, _channel);

				SetAudioPosition();
				ResumeAudio(_channel);
			}
		}
		//Previously in 3D mode, just changed into 2D and it is playing
		else
		{
			if (_play)
			{
				//Stop the original 3D Sound and change sound into 2D mode
				StopAudio(_channel);
				SetMode(_sound, FMOD_2D);
				PlayAudio(AUDIO->GetSystem(), _sound, _channel);
				ResumeAudio(_channel);
			}
		}
	}

	void ComponentAudio::SetAudioPosition()
	{
		FwMath::Vector3D vel = { 0.0f,0.0f,0.0f };
		FwMath::Vector3D pos = GetTransform()->_currentPosition;
		Set3DPosition(_channel, pos, vel);
	}

	void ComponentAudio::SetAudioMinMax()
	{
		StopAudio(_channel);
		Set3DMinMax(_sound, _min, _max);
		PlayAudio(AUDIO->GetSystem(), _sound, _channel);
		ResumeAudio(_channel);
	}

	ComponentTransform* ComponentAudio::GetTransform()
	{
		if (!_transform)
		{
			_transform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		}

		return _transform;
	}

	void ComponentAudio::SetTransform(ComponentTransform* transform)
	{
		_transform = transform;
	}

	void ComponentAudio::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		//Get the audio datas
		int i = 0;
		while (GetFirst(paramValues, std::string(STRING_AUDIO_FILE).append(std::to_string(i))) != "0")
		{
			_audioFileNames.push_back(GetFirst(paramValues, std::string(STRING_AUDIO_FILE).append(std::to_string(i))));
			++i;
		}

		i = 0;
		while (GetFirst(paramValues, std::string(STRING_AUDIO_NAME).append(std::to_string(i))) != "0")
		{
			_audioNames.push_back(GetFirst(paramValues, std::string(STRING_AUDIO_NAME).append(std::to_string(i))));
			++i;
		}
		
		//Only if there is a data saved then take the saved data, if not use default
		if (paramValues.find(STRING_VOLUME) != paramValues.end())
		{
			_volume = std::stof(GetFirst(paramValues, STRING_VOLUME));
		}

		if (paramValues.find(STRING_MINIMUM) != paramValues.end())
		{
			_min = std::stof(GetFirst(paramValues, STRING_MINIMUM));
		}

		if (paramValues.find(STRING_MAXIMUM) != paramValues.end())
		{
			_max = std::stof(GetFirst(paramValues, STRING_MAXIMUM));
		}

		_mute = std::stoi(GetFirst(paramValues, STRING_MUTE));
		_3dSound = std::stoi(GetFirst(paramValues, STRING_3DSOUND));

		if (paramValues.find(STRING_DEFAULT) != paramValues.end())
		{
			_defaultAudio = GetFirst(paramValues, STRING_DEFAULT);
		}

		_transform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
	}

	void ComponentAudio::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentAudio& source = reinterpret_cast<ComponentAudio&>(sauce);

		_audioFileNames.resize(source._audioFileNames.size());
		_audioNames.resize(source._audioNames.size());

		std::copy(source._audioFileNames.begin(), source._audioFileNames.end(), _audioFileNames.begin());
		std::copy(source._audioNames.begin(), source._audioNames.end(), _audioNames.begin());

		_volume = source._volume;
		_min = source._min;
		_max = source._max;

		_mute = source._mute;
		_3dSound = source._3dSound;

		_defaultAudio = source._defaultAudio;

		_transform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());

	}

	void ComponentAudio::Update(float)
	{
	}

	void ComponentAudio::Free()
	{
		_play = false;
		_currentAudio.clear();
		_defaultAudio.clear();
		StopSound();
		_audioFileNames.clear();
		_audioNames.clear();
	}

	void ComponentAudio::Destroy()
	{
	}

	std::pair<std::string, ParamValueMap> ComponentAudio::GetParams()
	{
		ParamValueMap params;

		//Saves the audio datas
		size_t maxSize = _audioFileNames.size();
		for (size_t i = 0; i < maxSize; ++i)
		{
			params.emplace(std::string(STRING_AUDIO_FILE).append(std::to_string(i)), _audioFileNames[i]);
			params.emplace(std::string(STRING_AUDIO_NAME).append(std::to_string(i)),_audioNames[i]);
		}

		params.emplace(STRING_DEFAULT, _defaultAudio);
		params.emplace(STRING_VOLUME, std::to_string(_volume));
		params.emplace(STRING_MINIMUM, std::to_string(_min));
		params.emplace(STRING_MAXIMUM, std::to_string(_max));
		params.emplace(STRING_MUTE, std::to_string(_mute));
		params.emplace(STRING_3DSOUND, std::to_string(_3dSound));

		return { STRING_COMPONENT_AUDIO, params };
	}
}
