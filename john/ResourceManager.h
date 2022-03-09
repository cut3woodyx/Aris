/******************************************************************************/
/*!
\file		ResourceManager.h
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
#include <map>
#include <string>
#include <memory>
#include "FWMath.h"
#include "FwGraphics.h"
#include "GraphicENUM.h"
#include "ShaderSystem.h"
#include "fmod.hpp"
#include "FwAudio.h"

namespace FwEngine
{
	class ResourceManager
	{
	private:
		//Shader storage
		ShaderSystem* _shaders;
		//Storage for textures
		std::map<std::string, FwMath::UINT> _textureMap;
		//Storage for fonts
		//Multimap of different font types due to having different sizes.
		std::multimap<std::string, std::pair<FwMath::UINT, FontAtlas>> _fontMap;
		//Storage for meshes
		std::map<MeshType, MeshData> _meshMap;
		//Storage for audio
		std::map<std::string, FMOD::Sound*> _soundMap;

		//Private functions for loading into containers.
		void LoadTexture(const std::string& file);
		void LoadFont(const std::string& font, const FwMath::UINT pixel);
		void LoadSound(const std::string& audio, System& system);
		void LoadMesh(MeshType type, const MeshData& mesh);

	public:
		ResourceManager();
		~ResourceManager();

		void LoadShaders();
		ShaderSystem* GetShader();
		FwMath::UINT GetTexture(const std::string& file);
		std::string FindFileByTextureID(FwMath::UINT id);
		FontAtlas* GetFont(const std::string& font, const FwMath::UINT pixel);
		Sound GetSound(const std::string& audio, System& system);
		MeshData& GetMesh(MeshType type);
	};

	extern ResourceManager* RESOURCE;
}