#include "pch.h"
#include "CameraCompoent.h"
#include "Object.h"
#include "TransformComponent.h"
#include "Declare.h"
CameraCompoent::CameraCompoent()
{
	cameraInfo->camerTransform = owner->GetComponent<TransformComponent>(0);
//	projectionMatrix = DX::XMMatrixPerspectiveFovLH(cameraInfo->FovAngleY, , cameraInfo->Near, cameraInfo->Far);
}

CameraCompoent::~CameraCompoent()
{
}

void CameraCompoent::ComponentInitialize()
{
}

