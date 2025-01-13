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
	if (nullptr != camerObj && Object::ObjectType::Camera == camerObj->GetObjectType())
	{ // 오브젝트 타입이 카메라일 경우
		cameraInfo->cameraTransform = camerObj->GetComponent<TransformComponent>();
		SetProjection(cameraInfo->FovAngleY, cameraInfo->Near, cameraInfo->Far);
		UpdateViewMatrix();
	}
	else
	{ // 아니면 삭제
		delete GetOwner();
		owner = nullptr;
	}

}

void CameraCompoent::ComponentUpdate(const float _deltaTime)
{
	if (InputVector.Length() > 0.0f)
	{
		DXMath::Vector3 position = cameraInfo->cameraTransform->GetPosition();
		position += InputVector * cameraInfo->Speed * _deltaTime;

		cameraInfo->cameraTransform->SetPosition(position);
		UpdateViewMatrix();
		InputVector = DXMath::Vector3::Zero;
	}
}

void CameraCompoent::UpdateViewMatrix()
{
	DXMath::Vector3 position = cameraInfo->cameraTransform->GetWorldMatrix().Translation();
	DXMath::Vector3 forward = cameraInfo->cameraTransform->GetWorldForward();
	DXMath::Vector3 up = cameraInfo->cameraTransform->GetLocalUp();

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

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		AddInputVector(forward);
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		AddInputVector(-forward);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::A))
	{
		AddInputVector(-right);
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::D))
	{
		AddInputVector(right);
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

