#include "pch.h"
#include "D2DFont.h"
#include "Renderer.h"
#include <cstdarg>  // 가변 인자 처리
#include <stdexcept>
#include "Helper.h"
#include "FontManager.h"

D2DFont::D2DFont(const std::wstring _Dialog)
{
    Dialog = _Dialog;
    LoadFont(L"경기천년제목");
    CreateLayoutText(Dialog);

    D2DClass::GetD2DBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::LightPink));
    SetWriteTextFormat(FontManager::GetInstance()->FindFont(L"standard"));
    Render();
}

D2DFont::~D2DFont()
{
    if (!DWriteTextFormat)
    {
        DWriteTextFormat->Release();
    }

   DWriteTextLayout->Release();
}

void D2DFont::SetDialog(std::wstring _input)
{
    Dialog = _input;
    CreateLayoutText(Dialog);
}

void D2DFont::LoadFont(std::wstring _fontName) // 외부 파일 읽을수 있도록 수정이 필요
{
    HRESULT hr = D2DClass::GetDWriteFactory()->CreateTextFormat(
        _fontName.c_str(), // FontName 제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능 거기 이름 다음 해야됨
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        FontSize,   // New Font Size
        L"", //locale
        &DWriteTextFormat
    );

    if (FAILED(hr))
    {
        OutputDebugString(L"CreateTextFormat 실패");
    }
    DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    CreateLayoutText(Dialog);
}
//void D2DFont::Render()
//{
//    D2DClass::GetD2DDeviceContext()->DrawTextLayout(Pos, DWriteTextLayout, D2DClass::GetD2DBrush().Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
//}
void D2DFont::Render()
{
    D2DClass::GetD2DDeviceContext()->DrawText(Dialog.c_str(), Dialog.length(), DWriteTextFormat, Pos, D2DClass::GetD2DBrush().Get());
    std::cout << "텍스트렌더" << std::endl;
}

void D2DFont::CreateLayoutText(std::wstring detail)
{
    if (DWriteTextLayout)
    {
        DWriteTextLayout->Release();
        DWriteTextLayout = nullptr;
    }

    HRESULT hr = D2DClass::GetDWriteFactory()->CreateTextLayout(
        detail.c_str(),
        static_cast<UINT32>(detail.length()),
        DWriteTextFormat,
        BoxSize.width,
        BoxSize.height,
        &DWriteTextLayout
    );

    SetSize(FontSize, { 0, (unsigned int)detail.length() }); //텍스트를 변경하더라도 기존사이즈를 유지
    //DWriteTextLayout.set
    if (FAILED(hr))
    {
        OutputDebugString(L"CreateTextLayout 실패");
    }

    DWriteTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    DWriteTextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
void D2DFont::SetSize(float _FontSize, DWRITE_TEXT_RANGE textRange)
{
    FontSize = _FontSize;
    DWriteTextLayout->SetFontSize(FontSize, textRange);
}
void D2DFont::SetWriteTextFormat(IDWriteTextFormat* _IDWriteTextFormat)
{
    DWriteTextFormat = _IDWriteTextFormat;
    CreateLayoutText(Dialog);
}

//void D2DFont::TextDraw(const wchar_t* format, D2D1_RECT_F _rect, D2D1_COLOR_F _color, ...)
//{
//    // 텍스트 그리기
//    D2DClass::GetD2DBrush()->SetColor(_color);
//
//    D2DClass::GetD2DDeviceContext()->DrawText(
//        format,              // 텍스트 내용
//        lstrlen(format) + 1,
//        DWriteTextLayout,
//        _rect,
//        D2DClass::GetD2DBrush().Get());
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void SFont::Init()
//{
//    this->FontCreate();
//}
//
//void SFont::UnInit()
//{
//    spriteFont.reset();
//    spriteBatch.reset();
//}
//
//bool SFont::FontCreate()
//{
//    const std::wstring filename = L"
//
//
// Font/kulim.sfont";
//
//    try
//    {
//        spriteBatch = std::make_unique<DirectX::SpriteBatch>(D3DClass::GetD3DDeviceContext().Get());
//        spriteFont = std::make_unique<DirectX::SpriteFont>(D3DClass::GetD3DDevice().Get(), filename.c_str());
//        spriteBatch->SetViewport(D3DClass::GetViewport()->Get());
//
//        spriteFont->SetLineSpacing(14.0f);  // 매직 넘버는 상수로 대체 가능
//        spriteFont->SetDefaultCharacter('_');
//    }
//    catch (const std::exception& e)
//    {
//        std::cerr << "폰트 생성 실패: " << e.what() << std::endl;
//        return false;
//    }
//
//    return true;
//}
//
//
//void SFont::TextDraw(int x, int y, COLOR col, const wchar_t* format, ...)
//{
//    // 현재 DepthStencilState 저장
//    ID3D11DepthStencilState* prevDepthState = nullptr;
//    UINT stencilRef;
//    D3DClass::GetD3DDeviceContext().Get()->OMGetDepthStencilState(&prevDepthState, &stencilRef);
//
//    // 가변 인자 처리
//    wchar_t buffer[1024] = {};
//    va_list args;
//    va_start(args, format);
//    vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
//    va_end(args);
//
//    // SpriteBatch 상태 저장 모드 사용
//    // SpriteBatch를 사용한 텍스트 렌더링
//    /*
//        텍스트 렌더링은 보통 Z-버퍼(depth buffer)를 사용하지 않기 때문에 Depth Test가 꺼질 수 있습니다.
//        TextDraw를 호출한 후 Depth State를 원래대로 복원해야 합니다.
//    */
//    spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr);
//    spriteFont->DrawString(spriteBatch.get(), buffer, DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y)), DirectX::XMVECTORF32({ col.x, col.y, col.z, col.w }));
//    spriteBatch->End();
//
//    // DepthStencilState 복원
//    D3DClass::GetD3DDeviceContext().Get()->OMSetDepthStencilState(prevDepthState, stencilRef);
//}