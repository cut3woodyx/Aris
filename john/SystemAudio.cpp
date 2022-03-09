/******************************************************************************/
/*!
\file		SystemAudio.cpp
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
#include "SystemAudio.h"
#include "SystemGraphics.h"

namespace FwEngine
{
	SystemAudio* AUDIO = nullptr;

	SystemAudio::SystemAudio() : _audioSystem{ nullptr }, _audioPosition{0.0f, 0.0f, 0.0f}
	{
		AUDIO = this;
	}

	SystemAudio::~SystemAudio()
	{
		ExitSystem(_audioSystem);
	}

	System& SystemAudio::GetSystem()
	{
		return _audioSystem;
	}

	void SystemAudio::UpdateAudio(ComponentAudio* audio)
	{
		//Only if enabled then update
		if (audio->isEnabled())
		{
			if (audio->_play)
			{
				//If editor stops the loop, make sure the audio stops at the end of the current loop
				if (!audio->_loop)
				{
					audio->StopSound(false);
				}
				//Checks if audio is played finish, if finished, empty out all the variables
				if (!audio->IsPlaying())
				{
					audio->_play = false;
					audio->_currentAudio = "";
					audio->_defaultAudio = "";
					audio->_loop = false;
				}
			}
			//Default audio plays if component currently is not playing and there is a default audio assigned
			else
			{
				if (audio->_currentAudio.empty())
				{
					if (!audio->_defaultAudio.empty())
					{
						audio->Play();
					}
				}
			}
		}
	}

	void SystemAudio::Init()
	{
		InitializeSystem(_audioSystem);
	}

	void SystemAudio::Update(float)
	{
		FwMath::Vector3D vel = { 0.0f,0.0f,0.0f };

		//Temporary ear position based on camera pos
		_audioPosition = GRAPHICS->_cameraPos;
		//Set the ear position first before update the fmod system
		Set3DListener(_audioSystem, _audioPosition , vel);
		UpdateSystem(_audioSystem);

		//Loops through the audio components and update them
		ComponentAudio* audio = &_gameObjectPool->GetContainerPtr<ComponentAudio>()->at(0);
		GameObject* gameObject = _gameObjectPool->GetGameObject(0);
		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentAudio>()->size();

		for (size_t i = 0; i < compSize; ++audio, ++i, ++gameObject)
		{
			if (!gameObject->_isActive)
				continue;

			if (audio->isEnabled())
				UpdateAudio(audio);			
		}
	}

	void SystemAudio::Free()
	{
		//Loops through the audio components to disable and free them
		ComponentAudio* comp = &_gameObjectPool->GetContainerPtr<ComponentAudio>()->at(0);
		const size_t compSize = _gameObjectPool->GetContainerPtr<ComponentAudio>()->size();
		for (size_t i = 0; i < compSize;
			++i, ++comp)
		{
			comp->disable();
			comp->Free();
		}
		StopAllAudio(_audioSystem);
	}

	void SystemAudio::Destroy()
	{
	}
}