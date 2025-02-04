#include "pch.h"
#include "TextObject.h"
#include "D2DRenderComponent.h"
#include "Helper.h"

TextObject::TextObject(std::string_view _name, Object::ObjectType type) : Object(_name, type)
{

}

TextObject::~TextObject()
{

}

void TextObject::Initialize()
{
	CreateComponent<D2DRenderComponent>();
}



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