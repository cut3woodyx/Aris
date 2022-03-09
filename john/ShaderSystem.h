/******************************************************************************/
/*!
\file		ShaderSystem.h
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
#pragma warning(disable : 26812)
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#define GLEW_STATIC
#include <glew.h>
#include "DebugDiagnostic.h"
#include "FWMath.h"
#include <exception>

enum ShaderType
{
	COLOUR,
	TEXTURE,
	SPRITESHEET,
	STRING,
	PARTICLE,

	TOTAL_SHADER = 5
};

enum Shader
{
	VERTEX,
	FRAGMENT,
	PROGRAM
};

class ShaderSystem
{
private:
	bool CompileCheck(int shader, Shader type)
	{
		int success;
		char logOutput[1024];
		if (type != PROGRAM)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, logOutput);
				Debug_LogToFile("Resources/Logs/shader_compile_log.txt", logOutput);
				return false;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, logOutput);
				Debug_LogToFile("Resources/Logs/program_compile_log.txt", logOutput);
				return false;
			}
		}
		return true;
	}

public:
	FwMath::UINT _shaderProgramIDs[ShaderType::TOTAL_SHADER];
	ShaderType _currentShader;

	class ShaderException : public std::exception
	{
		std::string _what;
		/*!< Message carrying the error type*/
	public:
		ShaderException(const std::string& msg)
		{
			_what = msg;
		}
		const char* what() const noexcept
		{
			return _what.c_str();
		}
	};

	ShaderSystem() try :_currentShader{ TEXTURE }, _shaderProgramIDs{ 0 }
	{
		//std::vector<std::string> define;
		//define.emplace_back("#define DEBUG\n");

		CDEBUG_ASSERT(LoadShader(COLOUR, 
								"Resources/Shaders/colourShader.vert", 
								"Resources/Shaders/colourShader.frag")
								, "Colour shader failed to load.");

		CDEBUG_ASSERT(LoadShader(TEXTURE, 
								"Resources/Shaders/textureShader.vert", 
								"Resources/Shaders/textureShader.frag")
								, "Texture shader failed to load.");

		CDEBUG_ASSERT(LoadShader(SPRITESHEET,
								"Resources/Shaders/animationShader.vert", 
								"Resources/Shaders/animationShader.frag"), 
								"Animation shader failed to load.");

		CDEBUG_ASSERT(LoadShader(STRING,
								"Resources/Shaders/textShader.vert",
								"Resources/Shaders/textShader.frag"),
								"Text shader failed to load.");

		CDEBUG_ASSERT(LoadShader(PARTICLE,
								"Resources/Shaders/particleShader.vert",
								"Resources/Shaders/particleShader.frag"),
								"Particle shader failed to load.");
	}
	catch (...)
	{
		//CDEBUG_ASSERT throws an empty exception, catch here to deal with it by
		//throwing a shader exception out to the caller
		throw ShaderException("Failed");
	}

	~ShaderSystem()
	{
		for (FwMath::UINT ID : _shaderProgramIDs)
		{
			if(ID != 0)
				glDeleteShader(ID);
		}
	}

	bool LoadShader(ShaderType type, const char* vertexShader, const char* fragmentShader, const std::vector<std::string>& defines = std::vector<std::string>())
	{
		std::string vertex;
		std::string fragment;
		std::ifstream vertexFile;
		std::ifstream fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		const char* version = "#version 330 core\n";
		size_t versionLength = std::strlen(version);

		try
		{
			//Read the whole files in
			vertexFile.open(vertexShader);
			fragmentFile.open(fragmentShader);

			std::stringstream vertexSS, fragmentSS;
			vertexSS << vertexFile.rdbuf();
			fragmentSS << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertex = vertexSS.str();
			fragment = fragmentSS.str();
		}
		catch (std::ifstream::failure exc)
		{
			Debug_LogToFile("Resources/Logs/shader_error.txt", exc.what());
			return false;
		}

		const char* vertexCode;
		const char* fragmentCode;

		unsigned int vertexID = 0, fragmentID = 0;
		unsigned int programID = 0;

		//If define list is available
		if (!defines.empty())
		{
			//If there is already version header defined, remove them.
			auto vertFind = vertex.find(version);
			if (vertFind != std::string::npos)
			{
				vertex.erase(vertFind, versionLength);
			}

			auto fragFind = fragment.find(version);
			if (fragFind != std::string::npos)
			{
				fragment.erase(fragFind, versionLength);
			}

			vertexCode = vertex.c_str();
			fragmentCode = fragment.c_str();

			size_t definesCount = defines.size();

			//version codes, followed by the defines, followed by the shader code, therefore + 2
			const char** shaderCodes = new const char*[definesCount + 2];
			int* defineLengths = new int[definesCount + 2];

			//#version 330 core put in the front
			shaderCodes[0] = version;
			defineLengths[0] = (int)versionLength;

			//Populate the array with the defines
			for (int i = 0; i < definesCount; ++i)
			{
				shaderCodes[(size_t)i+1] = defines[i].c_str();
				defineLengths[(size_t)i+1] = (int)defines[i].length();
			}

			//Finally the code will be at the last element
			shaderCodes[definesCount+1] = vertexCode;
			defineLengths[definesCount+1] = (int)std::strlen(vertexCode);

			vertexID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexID, (int)definesCount+2, shaderCodes, defineLengths);
			glCompileShader(vertexID);
			if (!CompileCheck(vertexID, VERTEX))
			{
				delete[] shaderCodes;
				delete[] defineLengths;
				return false;
			}

			shaderCodes[definesCount+1] = fragmentCode;
			defineLengths[definesCount+1] = (int)std::strlen(fragmentCode);

			fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentID, (int)definesCount+2, shaderCodes, defineLengths);
			glCompileShader(fragmentID);
			if (!CompileCheck(fragmentID, FRAGMENT))
			{
				delete[] shaderCodes;
				delete[] defineLengths;
				return false;
			}
	
			programID = glCreateProgram();
			glAttachShader(programID, vertexID);
			glAttachShader(programID, fragmentID);
			glLinkProgram(programID);
			if (!CompileCheck(programID, PROGRAM))
			{
				delete[] shaderCodes;
				delete[] defineLengths;
				return false;
			}

			delete[] shaderCodes;
			delete[] defineLengths;
		}
		else
		{
			vertexCode = vertex.c_str();
			fragmentCode = fragment.c_str();

			vertexID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexID, 1, &vertexCode, NULL);
			glCompileShader(vertexID);
			if (!CompileCheck(vertexID, VERTEX))
			{
				return false;
			}

			fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentID, 1, &fragmentCode, NULL);
			glCompileShader(fragmentID);
			if (!CompileCheck(fragmentID, FRAGMENT))
			{
				return false;
			}

			programID = glCreateProgram();
			glAttachShader(programID, vertexID);
			glAttachShader(programID, fragmentID);
			glLinkProgram(programID);
			if (!CompileCheck(programID, PROGRAM))
			{
				return false;
			}
		}

		_shaderProgramIDs[type] = programID;

		glDetachShader(_shaderProgramIDs[type], vertexID);
		glDetachShader(_shaderProgramIDs[type], fragmentID);
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		return true;
	}
	void ChangeShader(ShaderType type)
	{
		_currentShader = type;
	}
	void UseShader()
	{
		glUseProgram(_shaderProgramIDs[_currentShader]);
	}

	void ExitShader()
	{
		glUseProgram(0);
	}

	//For now only this, can be expanded if needed more
	void SetUniformInt(const char* name, int value)
	{
		glUniform1i(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),value);
	}

	void SetUniformFloat(const char* name, float value)
	{
		glUniform1f(glGetUniformLocation(_shaderProgramIDs[_currentShader], name), value);
	}

	void SetUniformVec2(const char* name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),x,y);
	}

	void SetUniformVec3(const char* name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),x,y,z);
	}

	void SetUniformVec4(const char* name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),x,y,z,w);
	}

	void SetUniformVec2(const char* name, FwMath::Vector2D vec)
	{
		glUniform2fv(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),1,&vec[0]);
	}

	void SetUniformVec3(const char* name, FwMath::Vector3D vec)
	{
		glUniform3fv(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),1,&vec[0]);
	}

	void SetUniformVec4(const char* name, FwMath::Vector4D vec)
	{
		glUniform4fv(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),1,&vec[0]);
	}

	void SetUniformMat4(const char* name, const FwMath::Matrix4x4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(_shaderProgramIDs[_currentShader], name),1,GL_FALSE,&mat[0][0]);
	}
};