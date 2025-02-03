#include "pch.h"
#include "CSVLoader.h"

void CSVLoader::Reader(std::string_view _filePath)
{ 
	CSVReader reader(_filePath);

	for (auto& row : reader)
	{
		std::string name;
		if (row["name"].is_null())
		{
			name = "";
		}
		else
		{
			name = row["name"].get<std::string>();
		}


		int age;
		if (row["age"].is_null())
		{
			age = 1;
		}
		else
		{
			age = row["age"].get<int>();
		}

		std::string city;
		if (row["city"].is_null())
		{
			name = "";
		}
		else
		{
			name = row["city"].get<std::string>();
		}

		// 여기서 구조체 만들어서 처리하면 될거 같다
		std::cout << "이름: " << name << ", 나이: " << age << ", 도시: " << city << std::endl;
	}
}
