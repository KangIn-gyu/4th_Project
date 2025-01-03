#pragma once
#include <string>
#include <string_view>
#include <filesystem>

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

	static std::wstring  GetwstrBasePath(std::wstring_view _filePath)
	{
		std::filesystem::path filePath(_filePath);
		std::filesystem::path basePath = filePath.parent_path();
		return basePath.wstring();
	}

	static std::string  GetstrBasePath(std::string_view _filePath)
	{
		std::filesystem::path filePath(_filePath);
		std::filesystem::path basePath = filePath.parent_path();
		return basePath.string();
	}

	// 경로에서 마지막 '/' 이후의 문자열을 반환하는 함수
	static std::wstring GetFileNameFromPath(std::wstring_view _filePath)
	{
		std::filesystem::path filePath(_filePath);
		return filePath.filename().wstring();
	}

	static std::string GetFileNameFromPath(std::string_view _filePath)
	{
		std::filesystem::path filePath(_filePath);
		return filePath.filename().string();
	}
};
