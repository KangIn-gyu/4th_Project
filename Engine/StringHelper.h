#pragma once
#include <string>
#include <string_view>
#include <filesystem>

template<typename T>
concept StringOrWString = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;

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
	template<StringOrWString T>
	static T GetFileNameFromPath(std::wstring_view _filePath)
	{
		std::filesystem::path filePath(_filePath);
		if constexpr (std::is_same_v<T, std::wstring>) 
		{
			return filePath.filename().wstring(); // std::wstring 반환
		}
		else 
		{
			return filePath.filename().string(); // std::string 반환
		}
	}
};
