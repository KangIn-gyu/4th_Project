#include "pch.h"
#include "CameraObject.h"
#include "CameraCompoent.h"
#include "Declare.h"
#include "Engine.h" 

CameraObject::CameraObject(Object::ObjectType type) : Object(type)
{
	windowInfo = ENGINE->GetWindowInfo();
}

void CameraObject::Start()
{
	CreateComponent<CameraCompoent>();
}

void CameraObject::Update(const float _deltaTime)
{

}

std::pair<int, int> CameraObject::GetWindowSize() const
{
	if (nullptr != windowInfo)
	{
		return std::pair<int, int>(windowInfo->screenWidth, windowInfo->screenHeight);
	}
	return std::pair<int, int>(0, 0); // 없을 경우 일단 안터지게 이렇게 처리함. 추후 조정이 필요
}
