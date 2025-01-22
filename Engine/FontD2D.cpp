#include "pch.h"
#include "FontD2D.h"
#include "Renderer.h"
#include <cstdarg>  // 가변 인자 처리
#include <stdexcept>
#include "Helper.h"

namespace FontManager
{


    D2DFont* D2DFont::m_pInstance = nullptr;
    SFont* SFont::m_pInstance = nullptr;

    void Initialize()
    {
        D2DFont::GetInstance()->Init();
        SFont::GetInstance()->Init();
    }

    void Uninitialize()
    {
        D2DFont::DestroyInstance();
        SFont::DestroyInstance();
    }

    // D2DFont ======================================================
    D2DFont* D2DFont::GetInstance()
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new D2DFont();
        }

        return m_pInstance;
    }

    void D2DFont::DestroyInstance()
    {
        if (m_pInstance != nullptr)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

    void D2DFont::Init()
    {
        CreateIDWriteFactory();
        CreateTextFormat();
    }

    void D2DFont::UnInit()
    {
    }

    void D2DFont::CreateIDWriteFactory()
    {
        HR_T(DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory)
        ));
    }

    void D2DFont::CreateTextFormat()
    {
        HR_T(pDWriteFactory->CreateTextFormat(
            L"Cooper",              // 글꼴 이름
            NULL,                  // 글꼴 컬렉션 (NULL은 시스템 기본 사용)
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            32.0f,                 // 글꼴 크기
            L"",              // 로케일
            &pTextFormat
        ));

        // 텍스트를 수평 및 수직으로 중앙에 맞춥니다.
        pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    void D2DFont::TextDraw(const wchar_t* format, D2D1_RECT_F _rect, D2D1_COLOR_F _color, ...)
    {
        // 텍스트 그리기
        D3DClass::GetD2DBrush()->SetColor(_color);

        D3DClass::GetD2DDeviceContext()->DrawText(
            format,              // 텍스트 내용
            lstrlen(format) + 1,
            pTextFormat,
            _rect,
            D3DClass::GetD2DBrush().Get());
    }

    // SFont ======================================================

    SFont* SFont::GetInstance()
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new SFont();
        }
        return m_pInstance;
    }

    void SFont::DestroyInstance()
    {
        if (m_pInstance != nullptr)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

    void SFont::Init()
    {
        this->FontCreate();
    }

    void SFont::UnInit()
    {
        spriteFont.reset();
        spriteBatch.reset();
    }

    bool SFont::FontCreate()
    {
        const std::wstring filename = L"../Resource/Font/NaNum.sfont";

        try
        {
            spriteBatch = std::make_unique<DirectX::SpriteBatch>(D3DClass::GetD3DDeviceContext().Get());
            spriteFont = std::make_unique<DirectX::SpriteFont>(D3DClass::GetD3DDevice().Get(), filename.c_str());
            spriteBatch->SetViewport(D3DClass::GetViewport()->Get());

            spriteFont->SetLineSpacing(14.0f);  // 매직 넘버는 상수로 대체 가능
            spriteFont->SetDefaultCharacter('_');
        }
        catch (const std::exception& e)
        {
            std::cerr << "폰트 생성 실패: " << e.what() << std::endl;
            return false;
        }

        return true;
    }


    void SFont::TextDraw(int x, int y, COLOR col, const wchar_t* format, ...)
    {
        // 현재 DepthStencilState 저장
        ID3D11DepthStencilState* prevDepthState = nullptr;
        UINT stencilRef;
        D3DClass::GetD3DDeviceContext().Get()->OMGetDepthStencilState(&prevDepthState, &stencilRef);

        // 가변 인자 처리
        wchar_t buffer[1024] = {};
        va_list args;
        va_start(args, format);
        vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
        va_end(args);

        // SpriteBatch 상태 저장 모드 사용
        // SpriteBatch를 사용한 텍스트 렌더링
        /*
            텍스트 렌더링은 보통 Z-버퍼(depth buffer)를 사용하지 않기 때문에 Depth Test가 꺼질 수 있습니다.
            TextDraw를 호출한 후 Depth State를 원래대로 복원해야 합니다.
        */
        spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr);
        spriteFont->DrawString(spriteBatch.get(), buffer, DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y)), DirectX::XMVECTORF32({ col.x, col.y, col.z, col.w }));
        spriteBatch->End();

        // DepthStencilState 복원
        D3DClass::GetD3DDeviceContext().Get()->OMSetDepthStencilState(prevDepthState, stencilRef);
    }
}