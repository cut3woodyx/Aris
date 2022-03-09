/******************************************************************************/
/*!
\file		ComponentAnimation.cpp
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

#include <iostream>

#include "ComponentAnimation.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "SystemGraphics.h"
#include <iterator>

namespace FwEngine
{
	constexpr auto STRING_ANIMATION = "action";
	constexpr auto STRING_FRAMES = "frame";
	constexpr auto STRING_TIME = "time";
	constexpr auto STRING_MAXROW = "row";
	constexpr auto STRING_MAXCOLUMN = "column";
	constexpr auto STRING_DEFAULT = "default";
	constexpr auto STRING_REVERSE = "reverse";

	float ComponentAnimation::GetDeltaTime()
	{
		return _currentDT;
	}

	float ComponentAnimation::GetOffsetX()
	{
		return _offsetX;
	}

	float ComponentAnimation::GetOffsetY()
	{
		return _offsetY;
	}

	void ComponentAnimation::CalculateOffsets()
	{
		_offsetX = 1.0f / _maxColumn;
		_offsetY = 1.0f / _maxRow;
	}

	int ComponentAnimation::GetAnimationFrame()
	{
		return _currentFrame;
	}

	int ComponentAnimation::GetAnimationIndex()
	{
		if (_animationNames.empty())
		{
			return 0;
		}

		try
		{
			CDEBUG_ERROR_IF(_animationNames.size() <= _currentAnimation || _currentAnimation < 0, "Animation out of bound");
		}
		catch (...)
		{
			Debug_LogToFile("Resources/Logs/error_log.txt", std::string("Animation out of bound : ").append(std::to_string(_currentAnimation)).c_str());
			return 0;
		}
		return _currentAnimation;
	}

	int ComponentAnimation::GetAnimationStart()
	{
		return _startFrame;
	}

	int ComponentAnimation::GetAnimationEnd()
	{
		return _endFrame;
	}

	void ComponentAnimation::SetAnimationFrame(int value)
	{
		_currentFrame = value;
	}

	void ComponentAnimation::SetAnimationIndex(int value)
	{
		_currentAnimation = value;
	}

	void ComponentAnimation::SetAnimationStart(int value)
	{
		_startFrame = value;
	}

	void ComponentAnimation::SetAnimationEnd(int value)
	{
		_endFrame = value;
	}

	void ComponentAnimation::AddDeltaTime(float dt)
	{
		_currentDT += dt;
	}

	void ComponentAnimation::ResetDeltaTime()
	{
		_currentDT = 0.0f;
	}

	ComponentAnimation::ComponentAnimation() : _startFrame{ 0 }, _endFrame{ 0 },_offsetX{ 0.0f }, _offsetY{ 0.0f },
		_currentDT{ 0.0f }, _maxColumn{ 1 }, _maxRow{ 1 }, 
		_animationsFrames{}, _animationNames{}, _frameTimes{},
		_currentFrame{ 0 }, _currentAnimation{ 0 }, _currentAnimationName{}, _defaultAnimationName{},
		_loop{ false }, _play{ false }, _reverse{ false }
	{
	}

	ComponentAnimation::ComponentAnimation(GameObject* parent) : IComponent(parent),
		_startFrame{ 0 }, _endFrame{ 0 }, _offsetX{ 0.0f }, _offsetY{ 0.0f },
		_currentDT{ 0.0f }, _maxColumn{ 1 }, _maxRow{ 1 },
		_animationsFrames{}, _animationNames{}, _frameTimes{},
		_currentFrame{ 0 }, _currentAnimation{ 0 }, _currentAnimationName{}, _defaultAnimationName{},
		_loop{ false }, _play{ false }, _reverse{ false }
	{
	}


	ComponentAnimation::~ComponentAnimation()
	{
	}

	bool ComponentAnimation::IsPlaying()
	{
		return _play;
	}

	void ComponentAnimation::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;

		int row = std::stoi(GetFirst(paramValues, STRING_MAXROW));
		int column = std::stoi(GetFirst(paramValues, STRING_MAXCOLUMN));

		if (row)
			_maxRow = row;
		if (column)
			_maxColumn = column;

		//Get the animation datas
		for (int i = 0; i < row; ++i)
		{
			std::string animation = GetFirst(paramValues, 
				std::string(STRING_ANIMATION).append(std::to_string(i))
			);
			int frame = std::stoi(GetFirst(paramValues, 
				std::string(STRING_FRAMES).append(std::to_string(i))
			));
			float frameTime = std::stof(GetFirst(paramValues, 
				std::string(STRING_TIME).append(std::to_string(i))
			));

			_animationsFrames.emplace_back(frame);
			_animationNames.emplace_back(animation);
			_frameTimes.emplace_back(frameTime);
		}

		//Checks if there is a default animation playing
		if (paramValues.find(STRING_DEFAULT) != paramValues.end())
		{
			_defaultAnimationName = GetFirst(paramValues, STRING_DEFAULT);
		}

		_reverse = std::stoi(GetFirst(paramValues, STRING_REVERSE));

		CalculateOffsets();
	}

	void ComponentAnimation::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentAnimation& source = reinterpret_cast<ComponentAnimation&>(sauce);

		_animationsFrames.resize(source._animationsFrames.size());
		_animationNames.resize(source._animationNames.size());
		_frameTimes.resize(source._frameTimes.size());

		std::copy(source._animationsFrames.begin(), source._animationsFrames.end(), _animationsFrames.begin());
		std::copy(source._animationNames.begin(), source._animationNames.end(), _animationNames.begin());
		std::copy(source._frameTimes.begin(), source._frameTimes.end(), _frameTimes.begin());

		_defaultAnimationName = source._defaultAnimationName;

		_reverse = source._reverse;

		_maxRow = source._maxRow;
		_maxColumn = source._maxColumn;

		CalculateOffsets();
	}

	void ComponentAnimation::PlayAnimation(std::string name, bool loop, int start, int end, bool reverse)
	{
		//Checks if the given string is blank
		if (name.empty())
		{
			//If blank and there is a default animation, use it
			if (!_defaultAnimationName.empty())
			{
				name = _defaultAnimationName;
			}
			else
			{
				return;
			}
		}

		//If playing the same animation and the same frames, stop
		if (_currentAnimationName == name && _startFrame == start && _endFrame == end)
		{
			_loop = loop;
			return;
		}

		//Check if name exist in the animation data
		int index;
		auto it = std::find(_animationNames.begin(), _animationNames.end(), name);

		try
		{
			CDEBUG_ASSERT(it != _animationNames.end(), "Animation name not found.");
		}
		catch (...)
		{
			_play = false;
			return;
		}

		//Populate the other datas based on name
		_reverse = reverse;
		index = (int)std::distance(_animationNames.begin(), it);
		_currentAnimation = index;
		_currentAnimationName = name;

		//Non reverse state and no starting frame declaration, just start from 0
		if (!reverse)
		{
			if (start)
			{
				_currentFrame = start;
			}
			else
			{
				_currentFrame = 0;
			}
		}
		else
		{
			if (end)
			{
				_currentFrame = end;
			}
			else
			{
				_currentFrame = _animationsFrames[index];
			}	
		}
		_play = true;
		_loop = loop;
		_startFrame = start;
		_endFrame = end;
	}

	void ComponentAnimation::StopAnimation()
	{
		_currentAnimationName = "";
		_defaultAnimationName = "";
		_play = false;
		_loop = false;
		_reverse = false;
		_currentAnimation = 0;
		_currentFrame = 0;
	}

	void ComponentAnimation::Update(float)
	{
	}

	void ComponentAnimation::Free()
	{
		_animationsFrames.clear();
		_animationNames.clear();
		_frameTimes.clear();

		StopAnimation();
	}

	void ComponentAnimation::Destroy()
	{
	}
	std::pair<std::string, ParamValueMap> ComponentAnimation::GetParams()
	{
		ParamValueMap params;
		params.emplace(STRING_DEFAULT, _defaultAnimationName);
		params.emplace(STRING_REVERSE, std::to_string(_reverse));

		params.emplace(STRING_MAXROW, std::to_string(_maxRow));
		params.emplace(STRING_MAXCOLUMN, std::to_string(_maxColumn));

		for (int i = 0; i < _maxRow; ++i)
		{
			if (_animationNames.size() > i)
				params.emplace(std::string(STRING_ANIMATION).append(std::to_string(i)), _animationNames[i]);
			if (_animationsFrames.size() > i)
				params.emplace(std::string(STRING_FRAMES).append(std::to_string(i)), std::to_string(_animationsFrames[i]));
			if (_frameTimes.size() > i)
				params.emplace(std::string(STRING_TIME).append(std::to_string(i)), std::to_string(_frameTimes[i]));
		}

		return { STRING_COMPONENT_ANIMATION, params };
	}
}

