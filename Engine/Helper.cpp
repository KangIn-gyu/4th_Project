#include "pch.h"
#include "Helper.h"

std::wstring StringConverter::StringToWide(std::string_view str)
{
    return std::wstring(str.begin(), str.end());
}

std::string StringConverter::WideToString(std::wstring_view str)
{
    return std::string(str.begin(), str.end());
}
