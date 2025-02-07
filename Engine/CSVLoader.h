#pragma once
#include "csv.hpp"
#include "SingletonBase.h"
using namespace csv;

#define CSVLOADER CSVLoader::GetInstance()
class CSVLoader : public SingletonBase<CSVLoader>
{
	using CSVData = std::vector<std::pair<int, std::wstring>>;
	friend class SingletonBase<CSVLoader>;

public:
	std::vector<std::pair<int, std::wstring>>& CSVLoad(std::string_view _filePath);


private:
	CSVLoader() = default;
	~CSVLoader() = default;

public:

private:
	std::unordered_map<std::string, CSVData> csvDatas;
	std::string basePath = "Resource/";
};

