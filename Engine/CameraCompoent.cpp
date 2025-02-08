#include "pch.h"
#include "CameraCompoent.h"
#include "CameraObject.h"

#include "TransformComponent.h"
#include "Declare.h"
#include "Helper.h"
#include "DirectXInput.h"

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
		InputVector = DXMath::Vector3::Zero;
	}
	UpdateViewMatrix();

	if (cameraInfo->projectionUpdate == true)
	{
		UpdateProjection();
		cameraInfo->projectionUpdate = false;
	}
}

void CameraCompoent::UpdateViewMatrix()
{
	DXMath::Vector3 position = cameraInfo->cameraTransform->GetWorldMatrix().Translation();
	DXMath::Vector3 forward = cameraInfo->cameraTransform->GetWorldForward();
	DXMath::Vector3 up = cameraInfo->cameraTransform->GetLocalUp();
	
	if (DX::XMVector3Equal(forward, DX::XMVectorZero()))
	{
		forward = DXMath::Vector3(0.0f, 0.0f, 1.0f);
	}

	viewMatrix = DX::XMMatrixLookAtLH(position, position + forward, up);
}

void CameraCompoent::AddInputVector(const DXMath::Vector3& input)
{
	InputVector += input;
	InputVector.Normalize();
}

void CameraCompoent::UpdateProjection()
{
	CameraObject* camerObj = static_cast<CameraObject*>(owner);
	float aspectRatio = static_cast<float>(camerObj->GetWindowSize().first) / camerObj->GetWindowSize().second;
	projectionMatrix = DX::XMMatrixPerspectiveFovLH(cameraInfo->FovAngleY, aspectRatio, cameraInfo->Near, cameraInfo->Far);
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

void CameraCompoent::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	DXMath::Vector3 forward = GetForward();
	DXMath::Vector3 right = GetRight();
	DXMath::Vector3 up = cameraInfo->cameraTransform->GetLocalUp();

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		AddInputVector(forward);
	}
	else if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		AddInputVector(-forward);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::A))
	{
		AddInputVector(-right);
	}
	else if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D))
	{
		AddInputVector(right);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::Space))
	{	// E 키 - 위로 이동
		AddInputVector(up);
	}
	else if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::LeftShift))
	{	// Q 키 - 아래로 이동
		AddInputVector(-up);
	}
	
	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::Escape))
	{
		PostQuitMessage(0);
	}

	DXINPUT->mouse->SetMode(_MouseState.rightButton ? DX::Mouse::MODE_RELATIVE : DX::Mouse::MODE_ABSOLUTE);
	if (_MouseState.positionMode == DX::Mouse::MODE_RELATIVE)
	{
		DXMath::Vector3 delta = DXMath::Vector3(float(_MouseState.x), float(_MouseState.y), 0.f) * cameraInfo->RotationSpeed;
		// 구한 이동량으로 회전
		cameraInfo->cameraTransform->AddYaw(delta.x);
		cameraInfo->cameraTransform->AddPithc(delta.y);

		DXMath::Quaternion currentRotation = cameraInfo->cameraTransform->GetQuaternion();
		UpdateViewMatrix();
	}

	static int lastWheelDelta = 0;
	const DX::Mouse::State& mouseState = DXINPUT->mouse->GetState();
	int wheelDelta = mouseState.scrollWheelValue;
	if (wheelDelta != lastWheelDelta) {
		if (wheelDelta > lastWheelDelta) {
			std::cout << "마우스 휠업함 " << " ";  
			// count++;  
		}
		// 휠이 아래로 굴러갔을 때
		else if (wheelDelta < lastWheelDelta) {
			std::cout << "마우스 휠 다운함 " << " ";  
			// count--; 
		}
		lastWheelDelta = wheelDelta;
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

