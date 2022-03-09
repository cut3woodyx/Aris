/******************************************************************************/
/*!
\file		MapFactory.cpp
\project	Aris
\author 	Primary: Tan Yong Wee
\par    	email: yongwee.tan\@digipen.edu
\date   	December 3, 2019
\brief

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <iostream>
#include "MapFactory.h"
#include "SystemCustomSerialization.h"



namespace FwEngine
{
	void MapFactory::BuildMap(std::string fileName, std::vector<std::string>& textures, Map* map)
	{

		// ADAPTED CODE =====================================================================
		CustomTextSerializer readFromFile;
		readFromFile.OpenRead(fileName);

		readFromFile.CustomReadMapCSV(map->_width, map->_height, map->MAP_SCALE, map->_mapArray, map->_mapArrayCol, textures, map->_noColIndex);
		map->MAP_SCALE_HF = map->MAP_SCALE * 0.5f;
		map->MAP_SCALE_INVERSE = 1.0f / map->MAP_SCALE;
		map->MAP_SCALE_P = map->MAP_SCALE / 10000.0f;

		//std::cout << "Width :" << map->_width << std::endl;
		//std::cout << "Height :" << map->_height << std::endl;

		//for (int h = 0; h < map->_height; ++h)
		//{
		//	//std::cout << "Row " << h << ": ";
		//	for (int w = 0; w < map->_width; ++w)
		//	{
		//		std::cout << map->_mapArray[(size_t)h * map->_width + w] << " ";
		//	}
		//	std::cout << std::endl;
		//}



		// ADAPTED CODE =====================================================================

		/*
		TextSerializer readFromFile;
		readFromFile.OpenRead(fileName);

		std::string currentRead;
		std::string subStr;

		int width = 0;
		int height = 0;

		readFromFile.ReadString(currentRead);
		subStr = currentRead.substr(0, currentRead.find(','));
		width = stoi(subStr);
		std::cout << "Width :" << width << std::endl;

		readFromFile.ReadString(currentRead);
		subStr = currentRead.substr(0, currentRead.find(','));
		height = stoi(subStr);
		std::cout << "Height :" << height << std::endl;

		map->height = height;
		map->width = width;

		if (map->mapArray)
			delete[] map->mapArray;

		size_t size = height * width;
		map->mapArray = new int[size];

		for (int i = 0; i < height; i++)
		{
			readFromFile.ReadString(currentRead);
			std::cout << "Row " << i << " :";

			for (int j = 0; j < width; j++)
			{
				subStr = currentRead.substr(0, currentRead.find(','));
				currentRead = currentRead.substr(currentRead.find(',')+1);

				int cell = stoi(subStr);

				std::cout << " " << cell;

				map->mapArray[i* width + j] = cell;
			}
			std::cout << std::endl;

		}
		*/
	}
}