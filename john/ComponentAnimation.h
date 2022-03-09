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
#pragma once

#include "IComponent.h"
#include <map>
#include <vector>
#include "ShaderSystem.h"

namespace FwEngine
{
	constexpr auto STRING_COMPONENT_ANIMATION = "animation";
	class ComponentAnimation : public IComponent
	{
	private:
		int _startFrame;
		int _endFrame;

		int _currentFrame;
		int _currentAnimation;

		float _offsetX;
		float _offsetY;

		float _currentDT;

	public:
		int _maxColumn;
		int _maxRow;

		//Containers to hold animation datas such as total frame per row, the name to use for corresponding row
		//and the time required to change frame per row
		std::vector<int> _animationsFrames;
		std::vector<std::string> _animationNames;
		std::vector<float> _frameTimes;

		//For editor to do testing
		std::string _currentAnimationName;
		std::string _defaultAnimationName;

		bool _loop;
		bool _play;
		bool _reverse;

		float GetDeltaTime();
		float GetOffsetX();
		float GetOffsetY();
		void CalculateOffsets();

		int GetAnimationFrame();
		int GetAnimationIndex();
		int GetAnimationStart();
		int GetAnimationEnd();

		void SetAnimationFrame(int value);
		void SetAnimationIndex(int value);
		void SetAnimationStart(int value);
		void SetAnimationEnd(int value);

		void AddDeltaTime(float dt);
		void ResetDeltaTime();

		ComponentAnimation();
		ComponentAnimation(GameObject* parent);
		~ComponentAnimation();

		bool IsPlaying();

		void Init(ParamValueMap& paramValues) override;
		void Clone(IComponent& sauce) override;
		void PlayAnimation(std::string name = "", bool loop = true, int start = 0, int end = 0, bool reverse = false);
		void StopAnimation();
		void Update(float dt) override;
		void Free() override;
		void Destroy() override;

		virtual std::pair<std::string, ParamValueMap> GetParams();
	};

}

