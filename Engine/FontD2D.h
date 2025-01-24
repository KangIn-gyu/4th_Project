#pragma once
#include "SingletonBase.h"
typedef DirectX::XMFLOAT4 COLOR;

    class D2DFont:public SingletonBase<D2DFont>
    {
    private:
        D2DFont() = default;
        ~D2DFont() { this->UnInit(); }
        D2DFont(const D2DFont&) = delete;
        D2DFont& operator =(const D2DFont&) = delete;

        static D2DFont* m_pInstance;

    public:

        void Init();
        void UnInit();

        void CreateIDWriteFactory();
        void CreateTextFormat();
        void TextDraw(const wchar_t* format, D2D1_RECT_F _rect, D2D1_COLOR_F _color = D2D1::ColorF(D2D1::ColorF::Black), ...);

    public:
        IDWriteFactory* pDWriteFactory = nullptr;
        IDWriteTextFormat* pTextFormat = nullptr;
    };


    // SFont ======================================================

    class SFont :public SingletonBase<SFont>
    {
    private:
        SFont() = default;
        ~SFont() { this->UnInit(); }
        SFont(const SFont&) = delete;
        SFont& operator =(const SFont&) = delete;

        static SFont* m_pInstance;

    public:

        void Init();
        void UnInit();

        bool FontCreate();
        void TextDraw(int x, int y, COLOR col, const wchar_t* format, ...);

    public:
        std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
        std::unique_ptr<DirectX::SpriteFont> spriteFont = nullptr;
    };