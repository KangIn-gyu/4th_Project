#include "pch.h"
#include "CSVLoader.h"

void CSVLoader::Reader(std::string_view _filePath)
{ 
	CSVReader reader(_filePath);
	CSVData sceneData;

	for (auto& row : reader)
	{
		std::string name;
		if (row["Name"].is_null())
		{
			name = "";
		}
		else
		{
			name = row["Name"].get<std::string>();
		}

		std::string text;
		if (row["Text"].is_null())
		{
			text = "";
		}
		else
		{
			text = row["Text"].get<std::string>();
		}
		sceneData.emplace_back(name, text);
	}

	csvDatas[std::string(_filePath)] = std::move(sceneData);
}

std::vector<std::pair<std::string, std::string>> CSVLoader::FindData(std::string_view _filePath)
{
	auto it = csvDatas.find(_filePath.data());
	
	if (it != csvDatas.end())
	{
		return it->second;
	}
	else
	{
		std::cout << "CSVLoader::FindData ¿À·ù\n";
	}
}
