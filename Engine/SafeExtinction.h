#pragma once

template <typename T>
class SafeExtinction
{
    static void SAFE_RELEASE(T* p)
    {
        if (p)
        {
            p->Release();
            p = nullptr;
        }
    }

    static void SAFE_DELETE(T* p)
    {
        if (p)
        {
            delete p;
            p = nullptr;
        }
    }
};