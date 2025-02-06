#include "pch.h"
#include "D2DBitMapFontScript.h"

#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

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
		ownerD2D->SceneCSVDataLoad(static_cast<D2DBaseObj*>(ownerObject)->CsvFilePath); // CSV처리
	}
}

void D2DBitMapFontScript::Update(const float _deltaTime)
{

}

void D2DBitMapFontScript::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{

}
