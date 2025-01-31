#pragma once
#include <dwrite_3.h>
#include "Component.h"

class D2DFont : public Component
    {
    public:
        D2DFont(std::wstring _Dialog);
        ~D2DFont();
        void SetDialog(std::wstring input);
        void LoadFont(std::wstring FontName);
        void Render();
        void CreateLayoutText(std::wstring detail);
        void SetSize(float _FontSize, DWRITE_TEXT_RANGE textRange);
        void SetWriteTextFormat(IDWriteTextFormat* _IDWriteTextFormat);

        IDWriteTextLayout* GetFontLayout() { return DWriteTextLayout; }
       // void TextDraw(const wchar_t* format, D2D1_RECT_F _rect, D2D1_COLOR_F _color, ...);

    public:

    private:
        std::wstring Dialog;
        IDWriteTextFormat* DWriteTextFormat = nullptr;
        IDWriteTextLayout* DWriteTextLayout = nullptr;
        float FontSize = 32.0f;
        D2D_SIZE_F BoxSize = { 100.f, 50.f };
        float x = 300, y = 300;
        D2D1_RECT_F Pos = { x , y ,x+ BoxSize .width,y + BoxSize.height };

        std::shared_ptr<D2DFont> d2dFontData;
    };


    // SFont ======================================================

    //class SFont :public SingletonBase<SFont>
    //{
    //private:
    //    SFont() = default;
    //    ~SFont() { this->UnInit(); }
    //    SFont(const SFont&) = delete;
    //    SFont& operator =(const SFont&) = delete;

    //    static SFont* m_pInstance;

    //public:

    //    void Init();
    //    void UnInit();

    //    bool FontCreate();
    //    void TextDraw(int x, int y, COLOR col, const wchar_t* format, ...);

    //public:
    //    std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
    //    std::unique_ptr<DirectX::SpriteFont> spriteFont = nullptr;
    //};