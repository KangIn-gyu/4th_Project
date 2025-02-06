#include "pch.h"
#include "D2DBitMapFontScript.h"

#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

void D2DBitMapFontScript::ComponentSetting()
{// 걍 하드 코딩 함
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		std::vector<std::string>& bitmapFilePath = static_cast<D2DBaseObj*>(ownerObject)->bitmapFilePath;
		for (int i = 0; i < bitmapFilePath.size(); i++)
		{
			ownerObject->GetComponent<D2DRenderComponent>()->Load2DImage(bitmapFilePath[i]);
		}
	}
}

void D2DBitMapFontScript::Update(const float _deltaTime)
{

}
