#include "pch.h"
#include "D2DBitMapFontScript.h"

#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/Helper.h"
void D2DBitMapFontScript::ComponentSetting()
{// 걍 하드 코딩 함
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		ownerD2D = ownerObject->GetComponent<D2DRenderComponent>();
		std::vector<std::string>& bitmapFilePath = static_cast<D2DBaseObj*>(ownerObject)->bitmapFilePath; // 비트맵 처리
		for (int i = 0; i < bitmapFilePath.size(); i++)
		{
			ownerD2D->Load2DImage(bitmapFilePath[i]);
		}
		ownerD2D->LoadFont(static_cast<D2DBaseObj*>(ownerObject)->fontFilePath); // 폰트로드
		std::wstring text = L"안녕하세요";
		{ // 폰트 초기화 예시
			ownerD2D->SetDialog(text);
			ownerD2D->SetTextSize(20.0f, { 0, static_cast<UINT32>(text.length()) }); // 사이즈 변경
			ownerD2D->SetFontPos(100, 100);
			ownerD2D->SetFontBoxSize(400, 400);
			ownerD2D->SetFontColor(D2D1::ColorF(D2D1::ColorF::White));
		}
	
		ownerD2D->SceneCSVDataLoad(static_cast<D2DBaseObj*>(ownerObject)->CsvFilePath); // CSV처리

	}
}

void D2DBitMapFontScript::Update(const float _deltaTime)
{

}

void D2DBitMapFontScript::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	std::vector<std::pair<int, std::wstring>>& csvData = ownerD2D->GetCSVDData(); // 복사가 안일어나게 처리

	if (_MouseTracker.rightButton == DX::Mouse::ButtonStateTracker::PRESSED)
	{
		int ChangeBitmapindex = csvData[index].first;
		ownerD2D->ChangeBitmap(ChangeBitmapindex);
		ownerD2D->SetDialog(csvData[index].second);
		index++;
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::Space))
	{
		
	}
}
