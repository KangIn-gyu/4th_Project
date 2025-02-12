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
	DXMath::Quaternion currentRotation = cameraInfo->cameraTransform->GetQuaternion();

	// 쿼터니언을 오일러 각으로 변환 (Yaw, Pitch, Roll)
	DXMath::Vector3 euler = currentRotation.ToEuler();
	
	euler.x = std::clamp(euler.x, -0.5f, 0.5f);
	euler.z = 0.0f;
	DXMath::Quaternion limitedRotation = DXMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
	cameraInfo->cameraTransform->SetQuaternion(limitedRotation);
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
	if(title)
		viewMatrix = DX::XMMatrixLookAtLH(position, lookat, up);
	else	
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

void CameraCompoent::LookAt(const DXMath::Vector3& _targetPosition)
{
	if (cameraInfo && cameraInfo->cameraTransform)
	{
		//DXMath::Vector3 position = cameraInfo->cameraTransform->GetPosition();
		//DXMath::Vector3 forward = _targetPosition - position;
		//forward.Normalize();  // 정규화하여 방향 벡터로 변환
		//
		//DXMath::Vector3 upVector(0.0f, 1.0f, 0.0f);  // 월드 업 벡터
		//DXMath::Vector3 right = upVector.Cross(forward);
		//right.Normalize();
		//
		//DXMath::Vector3 up = forward.Cross(right);
		//up.Normalize();
		//
		//// 카메라 변환 설정
		//DXMath::Matrix lookAtMatrix = DXMath::Matrix(
		//	right.x, up.x, forward.x, 0.0f,
		//	right.y, up.y, forward.y, 0.0f,
		//	right.z, up.z, forward.z, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f
		//);
		//
		//// 행렬을 쿼터니언으로 변환
		//DXMath::Quaternion rotation = DX::XMQuaternionRotationMatrix(lookAtMatrix);
		//
		//// 기존 트랜스폼 회전 함수 사용
		//cameraInfo->cameraTransform->SetQuaternion(rotation);
		lookat = _targetPosition;
		//UpdateViewMatrix();  // 뷰 행렬 갱신
	}

}

void CameraCompoent::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	DXMath::Vector3 forward = GetForward();
	DXMath::Vector3 right = GetRight();
	DXMath::Vector3 up = cameraInfo->cameraTransform->GetLocalUp();
	forward.y = 0.0f;
	if (true == movingFlag)
	{
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
			//AddInputVector(up);
		}
		else if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::LeftShift))
		{	// Q 키 - 아래로 이동
			//AddInputVector(-up);
		}

		if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::Escape))
		{
			PostQuitMessage(0);
		}
		
		
		//DXINPUT->mouse->SetMode(_MouseState.rightButton ? DX::Mouse::MODE_RELATIVE : DX::Mouse::MODE_ABSOLUTE);
		//DXINPUT->mouse->SetMode(_KeyState.IsKeyUp(DirectX::Keyboard::Keys::LeftAlt) ? DX::Mouse::MODE_RELATIVE : DX::Mouse::MODE_ABSOLUTE);
		if (_MouseState.positionMode == DX::Mouse::MODE_RELATIVE)
		{
			DXMath::Vector3 delta = DXMath::Vector3(float(_MouseState.x), float(_MouseState.y), 0.f) * cameraInfo->RotationSpeed;
			// 구한 이동량으로 회전
			//std::cout << delta.y << std::endl;
			cameraInfo->cameraTransform->AddYaw(delta.x);
	
	
			cameraInfo->cameraTransform->AddPithc(delta.y);
			/*cameraInfo->cameraTransform->SetRotation(DXMath::Quaternion::CreateFromYawPitchRoll
			(cameraInfo->cameraTransform->GetYaw(), newPitch, cameraInfo->cameraTransform->GetRoll()));
			cameraInfo->cameraTransform->AddYaw(delta.x);*/

			UpdateViewMatrix();
		}

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

