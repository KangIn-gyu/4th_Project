#pragma once
#include <string>
#include <string_view>

// 문자열 변환 함수
class StringConverter
{
public:
	static std::wstring StringToWide(std::string_view str);
	static std::string  WideToString(std::wstring_view std);
};

