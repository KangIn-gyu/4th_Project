#include "pch.h"
#include "LoopImageChangeScript.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

void LoopImageChangeScript::ComponentSetting()
{
	if (typeid(*ownerObject) == typeid(D2DBaseObj))
	{
		ownerD2D = ownerObject->GetComponent<D2DRenderComponent>();
		D2DBaseObj* owner = static_cast<D2DBaseObj*>(ownerObject);

		bitmapFilePath = owner->bitmapFilePath;
		for (int i = 0; i < bitmapFilePath.size(); i++)
		{
			ownerD2D->Load2DImage(bitmapFilePath[i]);
		}
	}
}

void LoopImageChangeScript::Update(const float _deltaTime)
{
	static float elapsedTime = 0.0f;  // 시간 누적 변수
	const float interval = 0.25f;


	elapsedTime += _deltaTime;
	if (elapsedTime >= interval)
	{
		elapsedTime = 0.0f;  // 타이머 초기화
		index++;

		if (index == bitmapFilePath.size())
		{
			index = 0;
		}
		ownerD2D->ChangeBitmap(index);
	
	}
}

void LoopImageChangeScript::ResetInformation()
{
	ownerObject->SetActive(true);
	ownerD2D->Set2DImagePos(posX, posY);
}

void LoopImageChangeScript::Set2DImagePos(float _x, float _y)
{
	posX = _x; 
	posY = _y;
}
