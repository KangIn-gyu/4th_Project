#include "pch.h"
#include "JustFont.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

JustFont::JustFont(Object* _own) : Script(_own)
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

void JustFont::ComponentSetting()
{
	
}

void JustFont::Update(const float _deltaTime)
{
	ownerD2D->GetOwner()->GetName();
	if (fvalue)
	{
		std::wstring value;
		value = std::to_wstring(*fvalue);
		std::wstring str;
		value = value.substr(0, value.find(L'.') + 3);
		str = message + value;
		ownerD2D->SetDialog(str);
	}
	else if(value)
	{
		std::wstring val;
		val = std::to_wstring(*value);
		std::wstring str;
		str = message + val;
		ownerD2D->SetDialog(str);
	}
	else
	{
		ownerD2D->SetDialog(message);
	}
	
}

void JustFont::ResetInformation()
{
}

void JustFont::SetTextSize(float _FontSize, DWRITE_TEXT_RANGE _textRange)
{
	ownerD2D->SetTextSize(_FontSize, _textRange);
}

void JustFont::SetFontBoxSize(float _width, float _height)
{
	ownerD2D->SetFontBoxSize(_width, _height);
}

void JustFont::SetFontPos(float _X, float _Y)
{
	ownerD2D->SetFontPos(_X, _Y);
}

