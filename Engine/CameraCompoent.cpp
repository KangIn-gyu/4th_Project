#include "pch.h"
#include "CameraCompoent.h"
#include "CameraObject.h"

#include "TransformComponent.h"
#include "Declare.h"

CameraCompoent::CameraCompoent()
{
	cameraInfo = new CameraInfo;
	cameraInfo->camerTransform = owner->GetComponent<TransformComponent>(0);
	CameraObject* camerObj = dynamic_cast<CameraObject*>(owner);
	if (camerObj)
	{
		float aspectRatio = static_cast<float>(camerObj->GetWindowSize().first) / camerObj->GetWindowSize().second;
		projectionMatrix = DX::XMMatrixPerspectiveFovLH(cameraInfo->FovAngleY, aspectRatio,
														cameraInfo->Near, cameraInfo->Far);
	}
	
}

CameraCompoent::~CameraCompoent()
{
}

void CameraCompoent::ComponentInitialize()
{
}

