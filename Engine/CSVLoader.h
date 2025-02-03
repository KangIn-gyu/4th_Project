#pragma once
#include "csv.hpp"
using namespace csv;

// test 용 CSV UTP8로 하면 안됨 
// 고민 사항 구조체 하나 만들어서 csv 데이터 하나 만들고 그걸로 처리를 할가 
// 해당 구조체로 거기에 정보 저장해서 주는 형식으로 해야 겠다 생각이 됨.

class CSVLoader
{
public:
	CSVLoader() = default;
	~CSVLoader() = default;

	void Reader(std::string_view _filePath);
private:

public:

private:
	
};

