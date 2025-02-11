#include "pch.h"
#include "JustFont.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"


void JustFont::ComponentSetting()
{
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		ownerD2D = ownerObject->GetComponent<D2DRenderComponent>(); // 매번 GetComponent하기 귀찮아서
		std::string bitmapFilePath = static_cast<D2DBaseObj*>(ownerObject)->path;
		ownerD2D->Load2DImage(bitmapFilePath);
		DXMath::Vector2 pos = static_cast<D2DBaseObj*>(ownerObject)->pos;
		ownerD2D->Set2DImagePos(pos.x, pos.y);
		{ // 폰트 초기화 예시
			ownerD2D->LoadFont(static_cast<D2DBaseObj*>(ownerObject)->fontFilePath);  // 폰트로드
			ownerD2D->SetDialog(L"");
			//ownerD2D->SetTextSize(20.0f, { 0, static_cast<UINT32>(text.length()) }); // 사이즈 변경
			ownerD2D->SetTextSize(30);
			ownerD2D->SetFontPos(pos.x, pos.y);
			ownerD2D->SetFontBoxSize(ownerD2D->Get2DImageXY().x, ownerD2D->Get2DImageXY().y);
			ownerD2D->SetAlignment(D2DFont::Setting::MIDDLE, D2DFont::Setting::MIDDLE);
			ownerD2D->SetLineSpacing(45.f);
			ownerD2D->SetFontColor(D2D1::ColorF(D2D1::ColorF::White));
		}
	}
}

void JustFont::Update(const float _deltaTime)
{
	std::wstring message;
	//message = std::to_wstring(*value);
	//ownerD2D->SetDialog(message);
	
}

void JustFont::ResetInformation()
{
}
