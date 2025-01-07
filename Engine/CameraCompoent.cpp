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
	std::cout << "CameraCompoent 삭제" << std::endl;
	SafeExtinction::SAFE_DELETE(cameraInfo);
}

void CameraCompoent::ComponentInitialize()
{
	CameraObject* camerObj = static_cast<CameraObject*>(owner);
	if (nullptr != camerObj && Object::ObjectType::Camara == camerObj->GetObjectType())
	{
		cameraInfo->cameraTransform = camerObj->GetComponent<TransformComponent>();
		SetProjection(cameraInfo->FovAngleY, cameraInfo->Near, cameraInfo->Far);
		UpdateViewMatrix();
	}
	else
	{
		delete GetOwner();
		owner = nullptr;
	}

}

void CameraCompoent::ComponentUpdate(const float _deltaTime)
{
	if (InputVector.Length() > 0.0f)
	{
		cameraInfo->cameraTransform->SetPosition(InputVector * cameraInfo->Speed *_deltaTime);
		UpdateViewMatrix();
		InputVector = DXMath::Vector3::Zero;
	}
}

void CameraCompoent::UpdateViewMatrix()
{
	DXMath::Vector3 position = cameraInfo->cameraTransform->GetWorldMatrix().Translation();
	DXMath::Vector3 forward = cameraInfo->cameraTransform->GetWorldForward();
	DXMath::Vector3 up = cameraInfo->cameraTransform->GetLocalUp();

	if (auto lenghtForward = forward.Length(); lenghtForward < 0.0001f)
	{
		forward = DXMath::Vector3::Forward; // 기본 forward 벡터로 설정
	}

	if (auto lenghtUp = up.Length(); lenghtUp < 0.0001f)
	{
		up = DXMath::Vector3::Up; // 기본 up 벡터로 설정
	}
	viewMatrix = DX::XMMatrixLookAtLH(position, position + forward, up);
}

void CameraCompoent::AddInputVector(const DXMath::Vector3& input)
{
	InputVector += input;
	InputVector.Normalize();
}

DXMath::Matrix CameraCompoent::GetViewMatrix() const
{
	return viewMatrix;
}

void CameraCompoent::SetProjection(float _FovAngleY, float _Near, float _Far)
{
	cameraInfo->FovAngleY = _FovAngleY;
	cameraInfo->Near = _Near;
	cameraInfo->Far = _Far;

	CameraObject* camerObj = static_cast<CameraObject*>(owner);
	if (camerObj)
	{
		cameraInfo->cameraTransform = camerObj->GetComponent<TransformComponent>();

		float aspectRatio = static_cast<float>(camerObj->GetWindowSize().first) / camerObj->GetWindowSize().second;
		projectionMatrix = DX::XMMatrixPerspectiveFovLH(_FovAngleY, aspectRatio, _Near, _Far);
	}
}

void CameraCompoent::SetSpeed(const float _speed)
{
	cameraInfo->Speed = _speed;
}

void CameraCompoent::SetRotationSpeed(const float _speed)
{
	cameraInfo->RotationSpeed = _speed;
}

void CameraCompoent::OnInputProcess(const DX::Keyboard::State& KeyState, const DX::Keyboard::KeyboardStateTracker& KeyTracker, const DX::Mouse::State& MouseState, const DX::Mouse::ButtonStateTracker& MouseTracker)
{
	DXMath::Vector3 forward = GetForward();
	DXMath::Vector3 right = GetRight();

	if (KeyState.IsKeyDown(DX::Keyboard::Keys::W))
	{
		AddInputVector(forward);
	}
}

DXMath::Vector3 CameraCompoent::GetForward()
{
	return cameraInfo->cameraTransform->GetWorldForward();
}

DXMath::Vector3 CameraCompoent::GetRight()
{
	return cameraInfo->cameraTransform->GetWorldRight();
}

