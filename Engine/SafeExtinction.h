#pragma once

class SafeExtinction // ¼¿ÇÁ ¼Ò¸ê
{
public:
    template <typename T>
    static void SAFE_RELEASE(T* p)
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }

    template <typename T>
    static void SAFE_DELETE(T* p)
    {
        if (p)
        {
            delete p;
            p = nullptr;
        }
    }
};