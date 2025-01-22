#pragma once
#include "D3DClass.h"
typedef DirectX::XMFLOAT4 COLOR;
namespace FontManager
{
    void Initialize();
    void Uninitialize();

    // D2DFont ======================================================
    class D2DFont
    {
    private:
        D2DFont() = default;
        ~D2DFont() { this->UnInit(); }
        D2DFont(const D2DFont&) = delete;
        D2DFont& operator =(const D2DFont&) = delete;

        static D2DFont* m_pInstance;

    public:
        static D2DFont* GetInstance();
        static void DestroyInstance();

        void Init();
        void UnInit();

        void CreateIDWriteFactory();
        void CreateTextFormat();
        void TextDraw(const wchar_t* format, D2D1_RECT_F _rect, D2D1_COLOR_F _color = D2D1::ColorF(D2D1::ColorF::Black), ...);

    public:
        IDWriteFactory* pDWriteFactory = nullptr;
        IDWriteTextFormat* pTextFormat = nullptr;
        IDWriteTextFormat* pTextFormat2 = nullptr;
    };


    // SFont ======================================================
    class SFont
    {
    private:
        SFont() = default;
        ~SFont() { this->UnInit(); }
        SFont(const SFont&) = delete;
        SFont& operator =(const SFont&) = delete;

        static SFont* m_pInstance;

    public:
        static SFont* GetInstance();
        static void DestroyInstance();

        void Init();
        void UnInit();

        bool FontCreate();
        void TextDraw(int x, int y, COLOR col, const wchar_t* format, ...);


    public:
        std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
        std::unique_ptr<DirectX::SpriteFont> spriteFont = nullptr;
    };
}