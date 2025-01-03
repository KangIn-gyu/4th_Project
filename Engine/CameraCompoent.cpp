#include "pch.h"
#include "CameraCompoent.h"
#include "CameraObject.h"

#include "TransformComponent.h"
#include "Declare.h"
#include "Helper.h"
CameraCompoent::CameraCompoent()
{
	cameraInfo = new CameraInfo;	
}

CameraCompoent::~CameraCompoent()
{
	SafeExtinction::SAFE_DELETE(cameraInfo);
}

void CameraCompoent::ComponentInitialize()
{
	cameraInfo->cameraTransform = owner->GetComponent<TransformComponent>(0);
	CameraObject* camerObj = static_cast<CameraObject*>(owner);
	if (camerObj)
	{
		float aspectRatio = static_cast<float>(camerObj->GetWindowSize().first) / camerObj->GetWindowSize().second;
		projectionMatrix = DX::XMMatrixPerspectiveFovLH(cameraInfo->FovAngleY, aspectRatio,
			cameraInfo->Near, cameraInfo->Far);
	}
}

