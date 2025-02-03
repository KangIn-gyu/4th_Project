#pragma once
#include <dwrite_3.h>
#include "Object.h"

class TextObject : public Object
    {
    public:
        TextObject(std::string_view _name, Object::ObjectType type = Object::ObjectType::UI, std::wstring _input =L"Basic");
        ~TextObject();
        void LoadFont(std::wstring FontName);
        void Render();
        void CreateLayoutText(std::wstring detail);
        void SetSize(float _FontSize, DWRITE_TEXT_RANGE textRange);
        void SetWriteTextFormat(IDWriteTextFormat* _IDWriteTextFormat);

        IDWriteTextLayout* GetFontLayout() { return DWriteTextLayout; }
        // void TextDraw(const wchar_t* format, D2D1_RECT_F _rect, D2D1_COLOR_F _color, ...);

    public:

        static TextObject* fontObject;
    private:
        std::wstring Dialog;
        IDWriteTextFormat* DWriteTextFormat = nullptr;
        IDWriteTextLayout* DWriteTextLayout = nullptr;
        float FontSize = 32.0f;
        D2D_SIZE_F BoxSize = { 100.f, 50.f };
        float x = 300, y = 300;
        D2D1_RECT_F Pos = { x , y ,x + BoxSize .width,y + BoxSize.height };

    };


    // SFont ======================================================

    //class SFont
    //{
    //private:
    //    SFont() = default;
    //    ~SFont() { this->UnInit(); }
    //    SFont(const SFont&) = delete;
    //    SFont& operator =(const SFont&) = delete;

    //public:

    //    void Init();
    //    void UnInit();

    //    bool FontCreate();
    //    void TextDraw(int x, int y, COLOR col, const wchar_t* format, ...);

    //public:
    //    std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
    //    std::unique_ptr<DirectX::SpriteFont> spriteFont = nullptr;
    //};