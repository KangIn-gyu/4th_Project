#include "pch.h"
#include "CameraObject.h"
#include "CameraCompoent.h"
#include "Declare.h"
#include "Engine.h" 

CameraObject* CameraObject::g_MainCameraObject = nullptr;

CameraObject::CameraObject(std::string_view _name, Object::ObjectType type) : Object(_name, type)
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

DXMath::Matrix CameraObject::GetProjectionMatrix() 
{
	return GetComponent<CameraCompoent>()->GetProjectionMatrix();
}

DXMath::Matrix CameraObject::GetViewMatrix()
{
	return GetComponent<CameraCompoent>()->GetViewMatrix();
}
