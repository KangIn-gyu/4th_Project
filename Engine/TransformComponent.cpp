#include "pch.h"
#include "TransformComponent.h"

#include "Object.h" //test

void TransformComponent::SetParent(TransformComponent* _parent)
{
	transform.SetParent(&_parent->transform);
}

void TransformComponent::AddPithc(const float _value)
{
	transform.AddPithc(_value);
}

void TransformComponent::AddYaw(const float _value)
{
	transform.AddYaw(_value);
}

TransformComponent::~TransformComponent()
{
	std::cout << "TransformComponent 삭제" << std::endl; // 추후 로그 시스템에 적용해야됨
}

void TransformComponent::UpdateTransform()
{
	transform.UpdateTransform();
}

void TransformComponent::ComponentUpdate(const float _deltaTime)
{
	transform.UpdateTransform();
}

DXMath::Matrix TransformComponent::GetWorldMatrix()
{
	return transform.GetWorldMatrix();
}

DXMath::Matrix TransformComponent::GetLocalMatrix() const
{
	return transform.GetLocalMatrix();
}

DXMath::Vector3 TransformComponent::GetLocalForward() const
{
	return transform.GetLocalForward();
}

DXMath::Vector3 TransformComponent::GetLocalUp() const
{
	return transform.GetLocalUp();
}

DXMath::Vector3 TransformComponent::GetLocalRight() const
{
	return transform.GetLocalRight();
}

DXMath::Vector3 TransformComponent::GetWorldForward() const
{
	return transform.GetWorldForward();
}

DXMath::Vector3 TransformComponent::GetWorldRight() const
{
	return transform.GetWorldRight();
}

DXMath::Vector3 TransformComponent::GetWorldUp() const
{
	return transform.GetWorldUp();
}

DXMath::Vector3 TransformComponent::GetWorldLook() const
{
	return transform.GetWorldLook();
}

DXMath::Vector3 TransformComponent::GetPosition() const
{
	return transform.GetPosition();
}

DXMath::Quaternion TransformComponent::GetQuaternion() const
{
	return transform.GetQuaternion();
}

DXMath::Vector3 TransformComponent::GetScale() const
{
	return transform.GetScale();
}

void TransformComponent::SetLocalMatrix(const DXMath::Matrix _localMatrix)
{
	transform.SetLocalMatrix(_localMatrix);
}

void TransformComponent::SetPosition(const DXMath::Vector3 _position)
{
	transform.SetPosition(_position);
}

void TransformComponent::SetQuaternion(const DXMath::Quaternion _rotation)
{
	transform.SetQuaternion(_rotation);
}

void TransformComponent::SetScale(const DXMath::Vector3 _scale)
{
	transform.SetScale(_scale);
}
