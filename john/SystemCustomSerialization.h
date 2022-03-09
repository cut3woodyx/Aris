/******************************************************************************/
/*!
\file		SystemCustomSerialization.h
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

#ifndef _SYSTEM_CUST_SERIALIZATION
#define _SYSTEM_CUST_SERIALIZATION

#include "SystemSerialization.h"
#include <unordered_map>

namespace FwEngine
{
	class CustomTextSerializer : public TextSerializer
	{
		typedef std::unordered_map<std::string, std::string> ParamMap;

		// CHANGE THESE IF YOU PLAN ON CHANGING THE FORMAT
		const char _start = '<';
		const char _end = '>';

		const char DELIMCHAR = '@';

		const std::string _breakName = "</>";
	public:
		CustomTextSerializer();
		CustomTextSerializer(const std::string& file);

		// Writes output to reference. Returns false if end of file.
		// Will clear the given map upon running.
		bool CustomReadObjComponent(std::string& gameObj, std::unordered_map<std::string, ParamMap>& compMap);

		void CustomSaveObjComponent(std::string filename,
			std::vector<std::pair<std::string, std::unordered_map<std::string, ParamMap>>> compMap);

		// Writes output to reference. Assumes mapArray will be reset.
		bool CustomReadMapCSV(int& width, int& height, float& scale,
			std::vector<int>& mapArray, std::vector<int>& mapArrayCol, std::vector<std::string>& textureArray, int& colIndex);

		void CustomSaveMapCSV(std::string filename, int& width, int& height, float& scale,
			std::vector<int>& mapArray, std::vector<int>& mapArrayCol, std::vector<std::string>& textureArray);

		~CustomTextSerializer() = default;
	};

}

#endif

