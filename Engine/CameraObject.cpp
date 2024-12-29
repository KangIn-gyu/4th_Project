#include "pch.h"
#include "CameraObject.h"

#include "Declare.h"

CameraObject::CameraObject()
{
}

CameraObject::~CameraObject()
{
}

void CameraObject::SetWindowInfo(WindowInfo* _windowInfo)
{
	windowInfo = _windowInfo;
}

std::pair<int, int> CameraObject::GetWindowSize() const
{
	if (nullptr != windowInfo)
	{
		return std::pair<int, int>(windowInfo->screenWidth, windowInfo->screenHeight);
	}
	return std::pair<int, int>(0, 0); // 없을 경우 일단 안터지게 이렇게 처리함. 추후 조정이 필요
}
