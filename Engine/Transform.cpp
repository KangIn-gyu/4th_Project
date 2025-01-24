#include "pch.h"
#include "Transform.h"

void Transform::UpdateTransform()
{
	DXMath::Matrix translationMatrix = DXMath::Matrix::CreateTranslation(position);
	DXMath::Matrix rotationMatrix = DXMath::Matrix::CreateFromQuaternion(rotation);
	DXMath::Matrix scaleMatrix = DXMath::Matrix::CreateScale(scale);
	localMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	if (nullptr != parent)
	{
		worldMatrix = localMatrix * parent->worldMatrix;
	}
	else
	{
		worldMatrix = localMatrix;
	}

	forward = -DXMath::Vector3::TransformNormal(DXMath::Vector3::Forward, worldMatrix);
	right = DXMath::Vector3::TransformNormal(DXMath::Vector3::Right, worldMatrix);
	up = DXMath::Vector3::TransformNormal(DXMath::Vector3::Up, worldMatrix);
	look = DXMath::Vector3::TransformNormal(DXMath::Vector3::Backward, worldMatrix);
}

DXMath::Matrix Transform::GetWorldMatrix() 
{
	if (nullptr != parent)
	{                                                                    
		worldMatrix = localMatrix * parent->worldMatrix;
	}
	else
	{
		worldMatrix = localMatrix;
	}

	return worldMatrix;
}

DXMath::Matrix Transform::GetLocalMatrix() const
{
	return localMatrix;
}

DXMath::Vector3 Transform::GetPosition() const
{
	return position;
}

DXMath::Quaternion Transform::GetQuaternion() const
{
	return rotation;
}

DXMath::Vector3 Transform::GetScale() const
{
	return scale;
}

DXMath::Vector3 Transform::GetLocalForward() const
{
	return  DXMath::Vector3::Transform(DXMath::Vector3::Forward, DXMath::Matrix::CreateFromQuaternion(rotation));
}

DXMath::Vector3 Transform::GetLocalUp() const
{
	return DXMath::Vector3::Transform(DXMath::Vector3::Up, DXMath::Matrix::CreateFromQuaternion(rotation));
}

DXMath::Vector3 Transform::GetLocalRight() const
{
	return DXMath::Vector3::Transform(DXMath::Vector3::Right, DXMath::Matrix::CreateFromQuaternion(rotation));
}

void Transform::SetLocalMatrix(const DXMath::Matrix _localMatrix)
{
	localMatrix = _localMatrix;

	// Position 추출
	position = DXMath::Vector3(localMatrix._41, localMatrix._42, localMatrix._43);

	// Scale 추출
	scale.x = DXMath::Vector3(localMatrix._11, localMatrix._12, localMatrix._13).Length();
	scale.y = DXMath::Vector3(localMatrix._21, localMatrix._22, localMatrix._23).Length();
	scale.z = DXMath::Vector3(localMatrix._31, localMatrix._32, localMatrix._33).Length();

	// Rotation 추출 (스케일 제거 후 쿼터니언 계산)
	DXMath::Matrix rotationMatrix = localMatrix;
	rotationMatrix._11 /= scale.x; rotationMatrix._12 /= scale.x; rotationMatrix._13 /= scale.x;
	rotationMatrix._21 /= scale.y; rotationMatrix._22 /= scale.y; rotationMatrix._23 /= scale.y;
	rotationMatrix._31 /= scale.z; rotationMatrix._32 /= scale.z; rotationMatrix._33 /= scale.z;

	rotation = DXMath::Quaternion::CreateFromRotationMatrix(rotationMatrix);
	
}

void Transform::SetPosition(const DXMath::Vector3 _position)
{
	position = _position;
	UpdateTransform();
}

void Transform::SetQuaternion(const DXMath::Quaternion _rotation)
{
	rotation = _rotation;
	UpdateTransform();
}

void Transform::SetScale(const DXMath::Vector3 _scale)
{
	scale = _scale;
	UpdateTransform();
}

void Transform::SetParent(Transform* _parent)
{
	if (nullptr != _parent && this != _parent)
	{
		parent = _parent;
	}
	else
	{
		std::cout << "Transform이 Null입니다" << '\n';
	}
}

void Transform::AddPithc(const float _value)
{
	// pitch 값 (x축 회전)을 추가합니다.

	DX::XMVECTOR qYaw = DX::XMQuaternionRotationAxis(DX::XMVectorSet(0, 1, 0, 0), DX::XMConvertToRadians(_value));
	// m_quaternion = DX::XMQuaternionMultiply(m_quaternion, qYaw);

	DXMath::Quaternion pitchRotation = DXMath::Quaternion::CreateFromAxisAngle(right, _value);
	rotation = DXMath::Quaternion::Concatenate(pitchRotation, rotation);

	// 회전 값이 -PI에서 PI 사이로 유지되도록 조정합니다.
	if (rotation.x > DX::XM_PI)
	{ 
		rotation.x -= DX::XM_2PI;
	}
	else if (rotation.x < -DX::XM_PI)
	{
		rotation.x += DX::XM_2PI;
	}
	UpdateTransform();
}

void Transform::AddYaw(const float _value)
{
	// yaw 값 (y축 회전)을 추가합니다.  // DXMath::Vector3::Up
	DXMath::Quaternion yawRotation = DXMath::Quaternion::CreateFromAxisAngle(DXMath::Vector3::Up, _value);
	rotation = DXMath::Quaternion::Concatenate(yawRotation, rotation);

	// 회전 값이 -PI에서 PI 사이로 유지되도록 조정합니다.
	if (rotation.y > DX::XM_PI)
	{
		rotation.y -= DX::XM_2PI;
	}
	else if (rotation.y < -DX::XM_PI)
	{
		rotation.y += DX::XM_2PI;
	}

	UpdateTransform();
}
