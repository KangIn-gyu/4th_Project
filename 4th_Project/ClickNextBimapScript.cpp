#include "pch.h"
#include "ClickNextBimapScript.h"

#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

void ClickNextBimapScript::ComponentSetting()
{// °Á ÇÏµå ÄÚµù ÇÔ
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		ownerD2D = ownerObject->GetComponent<D2DRenderComponent>(); // ¸Å¹ø GetComponentÇÏ±â ±ÍÂú¾Æ¼­
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
}

void ClickNextBimapScript::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	if (_MouseTracker.leftButton == DX::Mouse::ButtonStateTracker::PRESSED)
	{

		if (_MouseState.x <= 960 && index > 0 && index <= bitmapSize)
		{
			index--;
			ownerD2D->ChangeBitmap(index);
		}

		if (_MouseState.x > 960 && index >= 0 && index < bitmapSize)
		{
			index++;
			ownerD2D->ChangeBitmap(index);
		}

		if (_MouseState.x > 960 && index == bitmapSize && isClicked == true)
		{
			isClicked = false;
			ownerD2D->IsFadeIn = TRUE;
		}
	}
}
