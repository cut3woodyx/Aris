#include <fstream>
#include <algorithm>
#include "CSVReader.h"
#include <sstream>

namespace FwEngine
{
	std::vector<std::vector<std::string> > CSVReader::getData()
	{
		std::ifstream file(fileName);

		std::vector<std::vector<std::string> > dataList;

		std::string line = "";
		// Iterate through each line and split the content using delimeter
		while (getline(file, line))
		{
			std::vector<std::string> vec;
			split(vec, line, ',');
			dataList.push_back(vec);
		}
		// Close the File
		file.close();

		return dataList;
	}

	void CSVReader::split(std::vector<std::string>& vec, std::string line, char delimiter)
	{
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, delimiter))
		{
			vec.push_back(token);
		}
	}
}
