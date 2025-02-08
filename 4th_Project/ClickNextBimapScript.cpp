#include "pch.h"
#include "ClickNextBimapScript.h"

#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

void ClickNextBimapScript::ComponentSetting()
{// 걍 하드 코딩 함
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		ownerD2D = ownerObject->GetComponent<D2DRenderComponent>(); // 매번 GetComponent하기 귀찮아서
		std::vector<std::string>& bitmapFilePath = static_cast<D2DBaseObj*>(ownerObject)->bitmapFilePath;
		bitmapSize = bitmapFilePath.size();
		for (int i = 0; i < bitmapSize; i++)
		{
			ownerD2D->Load2DImage(bitmapFilePath[i]);
		}
	}
}

void ClickNextBimapScript::Update(const float _deltaTime)
{

}

void ClickNextBimapScript::ResetInformation()
{
	index = 0;
	ownerD2D->ChangeBitmap(index);
	state = State::Active;
}

void ClickNextBimapScript::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	if (_MouseTracker.leftButton == DX::Mouse::ButtonStateTracker::PRESSED)
	{
		if (index > 0 && index <= bitmapSize)
		{
			index--;
			ownerD2D->ChangeBitmap(index);
		}
	}

	if (_MouseTracker.rightButton == DX::Mouse::ButtonStateTracker::PRESSED)
	{
		if (index >= 0 && index < bitmapSize)
		{
			index++;
			ownerD2D->ChangeBitmap(index);
		}
	}
}
