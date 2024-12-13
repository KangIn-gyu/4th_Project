#pragma once
#include <string>
#include <string_view>

// 문자열 변환 함수
class StringConverter
{
public:
	static std::wstring StringToWide(std::string_view str)
	{
		return std::wstring(str.begin(), str.end());
	}
	static std::string  WideToString(std::wstring_view str)
	{
		return std::string(str.begin(), str.end());
	}
};
