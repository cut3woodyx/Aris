/******************************************************************************/
/*!
\file		ComponentText.cpp
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
#define NOMINMAX
#include <iostream>
#include "ComponentText.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "SystemGraphics.h"
#include "FwGraphics.h"
#include <math.h>

namespace FwEngine
{
	constexpr auto STRING_TYPE = "type";
	constexpr auto STRING_TEXT = "text";
	constexpr auto STRING_SCALE = "scale";
	constexpr auto STRING_RED = "red";
	constexpr auto STRING_GREEN = "green";
	constexpr auto STRING_BLUE = "blue";
	constexpr auto STRING_ALPHA = "alpha";

	ComponentText::ComponentText( ): _charMap{ nullptr }, _componentTransform{ nullptr },
									_pixel{ 0 }, _colour{ 0.0f,0.0f,0.0f,1.0f }, 
									_fontType{}, _displayText{}
	{
	}

	ComponentText::ComponentText(GameObject* parent) : IComponent(parent), _charMap{ nullptr }, _componentTransform{ nullptr },
		_pixel{ 0 }, _colour{ 0.0f,0.0f,0.0f,1.0f }, _fontType{}, _displayText{}
	{
	}


	ComponentText::~ComponentText()
	{
	}

	void ComponentText::SetFont(const std::string& type, FwMath::UINT pixel)
	{
		if (type.empty())
		{
			_charMap = nullptr;
			return;
		}

		_fontType = type;
		_pixel = pixel;
		try
		{
			_charMap = RESOURCE->GetFont(type, pixel);
		}
		catch (...)
		{
			_charMap = nullptr;
		}
	}

	FontAtlas* ComponentText::GetFont()
	{
		return _charMap;
	}

	ComponentTransform* ComponentText::GetTransform()
	{
		if (!_componentTransform)
		{
			_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		}

		return _componentTransform;
	}

	void ComponentText::SetTransform(ComponentTransform* transform)
	{
		_componentTransform = transform;
	}

	int ComponentText::CountLines()
	{
		std::string::const_iterator currentChar;;
		std::string::const_iterator end = _displayText.end();
		int lineCount = 1;

		//Counts the number of \n characters to get the number of lines
		for (currentChar = _displayText.begin(); currentChar != end; ++currentChar)
		{
			if ((*currentChar) == 0x0A)
			{
				++lineCount;
				continue;
			}
		}
		return lineCount;
	}

	float ComponentText::CalculateRowWidth(std::string::const_iterator it)
	{
		if (!_charMap)
		{
			return 0;
		}

		std::string::const_iterator end = _displayText.end();
		float width = 0.0f;

		//Counts the current width of the current row of characters
		for (; it != end; ++it)
		{
			if ((*it) == 0x0A)
			{
				return width;
			}
			width += _charMap->_chars[*it]._advance.x;
		}

		return width;
	}

	float ComponentText::CalculateMaxRowWidth()
	{
		if (!_charMap)
		{
			return 0;
		}

		//Counts the max width required for the whole string
		std::string::const_iterator end = _displayText.end();
		float width = 0.0f;
		float maxWidth = 0.0f;

		for (auto it = _displayText.begin(); it != end; ++it)
		{
			if ((*it) == 0x0A)
			{
				maxWidth = std::max(maxWidth, width);
				width = 0.0f;
			}
			width += _charMap->_chars[*it]._advance.x;
		}

		maxWidth = std::max(maxWidth, width);
		return maxWidth;
	}

	void ComponentText::Init(ParamValueMap& paramValues)
	{
		_isEnabled = true;
		_fontType = GetFirst(paramValues, STRING_TYPE);
		_displayText = GetFirst(paramValues, STRING_TEXT);

		//Replace all escape backslash followed by n character into linefeed character
		std::string temp = _displayText;
		size_t pos = 0;
		while ((pos = temp.find("\\n")) != std::string::npos)
		{
			temp.replace(pos, 2, "\n");
		}

		_displayText = temp;
		_pixel = std::stoi(GetFirst(paramValues, STRING_SCALE));
		
		float alpha = 1.0f;
		if (paramValues.find(STRING_ALPHA) != paramValues.end())
		{
			alpha = std::stof(GetFirst(paramValues, STRING_ALPHA));
		}

		_colour = FwMath::Vector4D(std::stof(GetFirst(paramValues, STRING_RED)),
								   std::stof(GetFirst(paramValues, STRING_GREEN)), 
								   std::stof(GetFirst(paramValues, STRING_BLUE)),
								   alpha);

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());
		
		try
		{
			_charMap = RESOURCE->GetFont(_fontType, _pixel);
		}
		catch (...)
		{
			//CDEBUG_PRINT(std::string(_fontType).append(" not found."));
			_charMap = nullptr;
		}
	}

	void ComponentText::Clone(IComponent& sauce)
	{
		_isEnabled = sauce.isEnabled();

		ComponentText& source = reinterpret_cast<ComponentText&>(sauce);

		_fontType = source._fontType;
		_displayText = source._displayText;
		_pixel = source._pixel;
		_colour = source._colour;

		_componentTransform = &_parentGameObject->GetParentObjectPool()->GetContainerPtr<ComponentTransform>()->at(_parentGameObject->GetIndex());

		try
		{
			_charMap = RESOURCE->GetFont(_fontType, _pixel);
		}
		catch (...)
		{
			//CDEBUG_PRINT(std::string(_fontType).append(" not found."));
			_charMap = nullptr;
		}

	}

	void ComponentText::Update(float)
	{
	}

	void ComponentText::Free()
	{
	}

	void ComponentText::Destroy()
	{
	}

	std::pair<std::string, ParamValueMap> ComponentText::GetParams()
	{
		ParamValueMap params;
		params.emplace(STRING_TYPE, _fontType);

		//Replace all line feed character into escape backslash followed by n
		//This is to allow saving of the text within 1 line in the save file
		std::string temp = _displayText;
		size_t pos = 0;
		while ((pos = temp.find('\n')) != std::string::npos)
		{
			temp.replace(pos, 1, std::string("\\").append("n"));
		}

		params.emplace(STRING_TEXT, temp);
		params.emplace(STRING_SCALE, std::to_string(_pixel));

		params.emplace(STRING_RED, std::to_string(_colour.x));
		params.emplace(STRING_GREEN, std::to_string(_colour.y));
		params.emplace(STRING_BLUE, std::to_string(_colour.z));
		params.emplace(STRING_ALPHA, std::to_string(_colour.w));

		return { STRING_COMPONENT_TEXT, params };
	}
}

