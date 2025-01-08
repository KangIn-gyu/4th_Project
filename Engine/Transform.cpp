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

	forward = DXMath::Vector3::TransformNormal(DXMath::Vector3::Forward, worldMatrix);
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

	if (nullptr != parent) 
	{
		worldMatrix = localMatrix * parent->worldMatrix;
	}
	else
	{
		worldMatrix = localMatrix;
	}
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
