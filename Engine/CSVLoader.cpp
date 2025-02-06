#include "pch.h"
#include "CSVLoader.h"
#include "Helper.h"

std::vector<std::pair<int, std::wstring>>& CSVLoader::CSVLoad(std::string_view _filePath)
{
	std::string filePath = basePath + _filePath.data();

	auto it = csvDatas.find(std::string(filePath));
	if(it != csvDatas.end())
	{
		return it->second;
	}

	CSVReader reader(filePath);
	CSVData sceneData;

	for (auto& row : reader)
	{
		int index = row["Index"].is_null() ? -1 : row["Index"].get<int>();

		std::string text;
		std::wstring maintext;
	// TODO : 문자열 변경에 대해서 고민중
		if (!row["Text"].is_null())
		{
			text = row["Text"].get<std::string>();
			std::u8string u8str_text(text.begin(), text.end());
			maintext = StringConverter::Utf8ToWString(u8str_text);
		}

		sceneData.emplace_back(index, std::move(maintext));
	}

	return csvDatas[std::string(filePath)] = std::move(sceneData);
}
