#pragma once
// 목적 : 함수 객체 헤더 모음 
// 기능별 자주 사용할 것 같은 함수를 함수 객체로 만들어서 처리


#include "StringHelper.h"
#include "SafeExtinction.h"

// D3D 컴객체 확인용
LPCWSTR GetComErrorString(HRESULT hr);
// Helper class for COM exceptions
class com_exception : public std::exception
{
public:
    com_exception(HRESULT hr) : result(hr) {}

    const char* what() const noexcept override
    {
        static char s_str[64] = {};
        sprintf_s(s_str, "Failure with HRESULT of %08X",
            static_cast<unsigned int>(result));
        return s_str;
    }

private:
    HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
inline void HR_T(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw com_exception(hr);
    }
}