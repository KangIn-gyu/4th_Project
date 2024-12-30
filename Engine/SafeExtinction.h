#pragma once

template <typename T>
concept HasRelease = requires(T t) 
{
    { t.Release() } -> std::same_as<void>;  //검사 표현식 Cpp 20버전 스타일
};

class SafeExtinction // 셀프 소멸
{
public:
    template <HasRelease T>
    static void SAFE_RELEASE(T* p)
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }

    template <typename T> requires std::is_pointer_v<T>
    static void SAFE_DELETE(T p) 
    {
        if (p)
        {
            delete p;
            p = nullptr;
        }
    }
};