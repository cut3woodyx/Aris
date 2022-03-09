/******************************************************************************/
/*!
\file		ResourceManager.cpp
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
#include "ResourceManager.h"

namespace FwEngine
{
	ResourceManager* RESOURCE = nullptr;

	ResourceManager::ResourceManager() : _shaders{ nullptr },
		_textureMap{}, _fontMap{}, _meshMap{}, _soundMap{}
	{
		RESOURCE = this;
	}

	ResourceManager::~ResourceManager()
	{
		if (_shaders)
		{
			delete _shaders;
		}

		for (auto texture : _textureMap)
		{
			DeleteTexture(texture.second);
		}

		for (auto fonts : _fontMap)
		{
			DeleteFont(fonts.second.second);
		}

		for (auto mesh : _meshMap)
		{
			DeleteBuffers(mesh.second);
		}

		/*for (auto& sound : _soundMap)
		{
			FreeAudio(sound.second);
		}*/
	}

	void ResourceManager::LoadShaders()
	{
		_shaders = new ShaderSystem();
	}

	ShaderSystem* ResourceManager::GetShader()
	{
		return _shaders;
	}

	void ResourceManager::LoadTexture(const std::string& file)
	{
		FwMath::UINT texture;
		GenerateTexture(texture, file);
		_textureMap[file] = texture;
	}

	FwMath::UINT ResourceManager::GetTexture(const std::string& file)
	{	
		if (file.empty() || file.find_first_not_of(' ') == std::string::npos)
			return 0;

		auto it = _textureMap.find(file);
		if (it != _textureMap.end())
		{
			return it->second;
		}
		else
		{
			LoadTexture(file);
		}

		return _textureMap[file];
	}

	std::string ResourceManager::FindFileByTextureID(FwMath::UINT id)
	{
		auto end = _textureMap.end();
		for (auto it = _textureMap.begin(); it != end; ++it)
		{
			if (it->second == id)
			{
				return it->first;
			}
		}
		return std::string();
	}

	void ResourceManager::LoadFont(const std::string& font, const FwMath::UINT pixel)
	{
		FontAtlas fonts;
		GenerateFont(font, pixel, fonts);
		_fontMap.insert(std::make_pair(font, std::make_pair(pixel, fonts)));
	}

	FontAtlas* ResourceManager::GetFont(const std::string& font, const FwMath::UINT pixel)
	{
		if (font.empty() || pixel == 0)
			return nullptr;

		auto begin = _fontMap.lower_bound(font);
		auto end = _fontMap.upper_bound(font);

		//Nothing found in the map jus load and return the only instance
		if (begin == end)
		{
			LoadFont(font, pixel);
			return &_fontMap.find(font)->second.second;
		}

		//Something is in the map but might not be the correct size
		while (begin != end)
		{
			if (begin->second.first == pixel)
			{
				return &begin->second.second;
			}

			++begin;
		}

		//Load font with new size if the size given is not loaded 
		LoadFont(font, pixel);

		//Find the range again and return the correct font data
		auto newBegin = _fontMap.lower_bound(font);
		auto newEnd = _fontMap.upper_bound(font);
		
		while (newBegin != newEnd)
		{
			if (newBegin->second.first == pixel)
			{
				return &newBegin->second.second;
			}

			++newBegin;
		}

		//Will never reach here placed here to get rid of warnings
		return nullptr;
	}

	void ResourceManager::LoadSound(const std::string& audio, System& system)
	{
		Sound tempSound;

		CreateAudio(system, tempSound, audio);
		CDEBUG_ASSERT(tempSound, std::string(audio).append(" is either not found or of invalid type.\nFailed to create sound!!").c_str());

		//DisableLoop(tempSound);
		_soundMap[audio] = tempSound;
	}

	Sound ResourceManager::GetSound(const std::string& audio, System& system)
	{
		if (audio.empty())
			return nullptr;

		auto it = _soundMap.find(audio);
		if (it != _soundMap.end())
		{
			return it->second;
		}
		else
		{
			LoadSound(audio, system);
		}

		return _soundMap[audio];
	}

	void ResourceManager::LoadMesh(MeshType type, const MeshData& mesh)
	{
		_meshMap[type] = mesh;
	}

	MeshData& ResourceManager::GetMesh(MeshType type)
	{
		auto it = _meshMap.find(type);
		if (it != _meshMap.end())
		{
			return it->second;
		}
		else
		{
			LoadMesh(type, MeshData());
		}

		return _meshMap[type];
	}
}