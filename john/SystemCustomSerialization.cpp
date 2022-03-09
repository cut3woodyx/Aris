/******************************************************************************/
/*!
\file		SystemCustomSerialization.cpp
\project	Aris
\author 	Primary: Leong Jun Ming
\par    	email: l.junming\@digipen.edu
\date   	December 03, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "SystemCustomSerialization.h"
#include <algorithm>

FwEngine::CustomTextSerializer::CustomTextSerializer()
	: TextSerializer()
{}

FwEngine::CustomTextSerializer::CustomTextSerializer(const std::string& file)
	: TextSerializer(file)
{}

bool FwEngine::CustomTextSerializer::CustomReadObjComponent(std::string& gameObj, std::unordered_map<std::string, ParamMap>& compMap)
{
	// If sudden EOF or bad file, End Immediately.
	// Returns false if it fails to fully read.
	if (!IsGood())
		return false;

	// Stores Values
	std::string read;
	std::string curComp;
	ParamMap cMap;

	// Number of breaks it needs to stop reading
	int nameBreaks = 1;

	// Read Name
	ReadStringLine(read);

	if (read.size() < 2)
		return false;

	// Object Name
	gameObj = read.substr(1, read.size()-2);

	// Clear Component Map data
	compMap.clear();

	// If can still read
	while (nameBreaks && IsGood())
	{
		ReadString(read);
		// Hits a break point in reading
		if (read == _breakName)
		{
			if (--nameBreaks > 0)
			{
				// Add Component Vars to Map
				compMap[curComp] = cMap;
				cMap.clear();
			}
		}
		// Not an end point
		else if (read.at(0) == _start && read.at(read.size()-1) == _end)
		{
			CDEBUG_ERROR_IF(read.size() < 2, "Component name cannot be read: Less than 2 characters.");
			if (nameBreaks > 1)
			{
				// Reads whole line 
				std::string par;
				ReadStringLine(par);
				par = par.substr(1, par.size());
				_stream.ignore();
				cMap[read.substr(1, read.size()-2)] = par;
			}
			else
			{
				curComp = read.substr(1, read.size() - 2);
				++nameBreaks;
			}
		}
	}

	_stream.ignore();

	return IsGood();
}

void FwEngine::CustomTextSerializer::CustomSaveObjComponent(std::string filename, 
	std::vector<std::pair<std::string, std::unordered_map<std::string, ParamMap>>> compMap)
{
	std::stringstream buff;

	for (std::pair<std::string, std::unordered_map<std::string, ParamMap>> comps : compMap)
	{
		// Name of Object
		buff << _start << comps.first << _end << "\n";
		for (std::pair<std::string, ParamMap> i : comps.second)
		{
			// Name of Component
			buff << "\t" << _start << i.first << _end << "\n";
			for (std::pair<std::string, std::string> j : i.second)
			{
				// Name of Component Variable
				buff << "\t\t" << _start << j.first << _end << " " << j.second << "\n";
			}
			buff << "\t" << _breakName << "\n";
		}
		buff << _breakName << "\n";
	}

	_buffer.clear();
	Write(buff.str());
	OpenWrite(filename);
}

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool FwEngine::CustomTextSerializer::CustomReadMapCSV(int& width, int& height, float& scale,
	std::vector<int>& mapArray, std::vector<int>& mapArrayCol, std::vector<std::string>& textureArray, int& colIndex)
{
	// If sudden EOF or bad file, End Immediately.
	// Returns false if it fails to fully read.
	if (!IsGood())
		return false;

	// Stores Values
	std::string read;

	// Read CSV Line
	ReadString(read);
	// Width
	width = std::stoi(read.substr(0, read.find(',')));
	ReadString(read);
	// Height
	height = std::stoi(read.substr(0, read.find(',')));
	ReadString(read);
	// Scale
	size_t p = read.find(',');
	scale = std::stof(read.substr(0, p));
	// Read Values of Tiles
	if (width != 0 && height != 0)
	{
		try 
		{
			read = read.substr(p + 1, read.length());
			colIndex = (int)std::stof(read.substr(0, read.find(',')));
		}
		catch (...)
		{	
		}
		
	}
	

	/*
	if (mapArray)
		delete[] mapArray;
	*/
	mapArray.clear();
	mapArrayCol.clear();

	// Casting to get rid of annoying compiler warning bug
	mapArray.resize((size_t)width * height); //= std::make_shared<int[]>(width*height);//new int[(size_t)width*height];
	mapArray.shrink_to_fit();

	mapArrayCol.resize((size_t)width * height);
	mapArrayCol.shrink_to_fit();

	// Read tile data
	for (int h = height - 1; h >= 0; --h)
	{
		ReadString(read);
		for (int w = 0; w < width; ++w)
		{
			std::string sub = read.substr(0, read.find(','));

			auto assertNum = [&](std::string astStr)
			{
				try {
					CDEBUG_ERROR_IF(!is_number(astStr),
						std::string("Located Error! Unidentified value in map. Please check the file. Location Error: ")
						.append(" Width: ").append(std::to_string(w + 1)).append(" Height: ").append(std::to_string(height - h + 2)).c_str());
				}
				catch (...)
				{
					mapArray.clear();
					height = 0;
					width = 0;
					return false;
				}
				return true;
			};

			read = read.substr(read.find(',')+1);

			//if ((w + 1) == 107 && (height - h + 2) == 3)
			//	int i = 1;

			try
			{
				// Get tile index and tile collidable
				size_t index = sub.find(DELIMCHAR);
				if (index == std::string::npos)
				{
					assertNum(sub);
					int tileIndex = stoi(sub);
					mapArray[(size_t)h * width + w] = tileIndex;
					mapArrayCol[(size_t)h * width + w] = (tileIndex == 0 ? 0 : 1);
				}
				else
				{
					std::string subIndex = sub.substr(0, sub.find(DELIMCHAR));

					assertNum(subIndex);

					int tileIndex = stoi(subIndex);

					sub = sub.substr(sub.find(DELIMCHAR) + 1);

					assertNum(sub);

					int tileCol = stoi(sub);

					mapArray[(size_t)h * width + w] = tileIndex;

					mapArrayCol[(size_t)h * width + w] = tileCol;
				}

			}
			catch (...)
			{
				try
				{
					CDEBUG_ERROR_IF(1, "Unable to Read Map properly - Map size is not correct");
				}
				catch (...)
				{
					mapArray.clear();
					height = 0;
					width = 0;
					return false;
				}
			}
		}
	}

	read.clear();
	ReadString(read);
	// Texture data
	while (!read.empty())
	{
		textureArray.push_back(read.substr(0, read.find(',')));
		read.clear();
		ReadString(read);
	}

	return true;
}

void FwEngine::CustomTextSerializer::CustomSaveMapCSV(std::string filename, 
	int& width, int& height, float& scale, std::vector<int>& mapArray, std::vector<int>& mapArrayCol, std::vector<std::string>& textureArray)
{
	std::stringstream buff;
	auto addComma = [](std::stringstream& buf, int num) {for (int i = 0; i < num; ++i)buf << ","; buf << "\n"; };
	buff << width;
	addComma(buff, width);
	buff << height;
	addComma(buff, width);
	buff << scale;
	addComma(buff, width);
	for (int h = height - 1; h >= 0; --h)
	{
		for (int w = 0; w < width; ++w)
		{
			buff << mapArray[(size_t)h * width + w] << DELIMCHAR << mapArrayCol[(size_t)h * width + w] << ",";
		}
		buff << "\n";
	}
	for (std::string i : textureArray)
	{
		buff << i;
		addComma(buff, width - 1);
	}
	Write(buff.str());
	OpenWrite(filename);
}
