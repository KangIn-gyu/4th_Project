#include "pch.h"
#include "CameraCompoent.h"
#include "CameraObject.h"

#include "TransformComponent.h"
#include "Declare.h"
#include "Helper.h"

CameraCompoent::CameraCompoent()
{
	cameraInfo = new CameraInfo;
	CameraObject* cameraOwner = dynamic_cast<CameraObject*>(GetOwner());
	if (nullptr == cameraOwner)
	{
		delete GetOwner();
		owner = nullptr;
	}
	// 테스트 코드
	std::cout << "카메라 컴포넌트 생성" << '\n';
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

void CameraCompoent::SetProjection(float _FovAngleY, float _Near, float _Far)
{
	cameraInfo->FovAngleY = _FovAngleY;
	cameraInfo->Near = _Near;
	cameraInfo->Far = _Far;

//	projectionMatrix = DX::XMMatrixPerspectiveFovLH()
}

