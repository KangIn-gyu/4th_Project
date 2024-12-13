#include "pch.h"
#include "TransformComponent.h"

void TransformComponent::UpdateTransform()
{
	DXMath::Matrix translationMatrix = DXMath::Matrix::CreateTranslation(position);
	DXMath::Matrix rotationMatrix = DXMath::Matrix::CreateFromQuaternion(rotation);
	DXMath::Matrix scaleMatrix = DXMath::Matrix::CreateScale(scale);
	localMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	if (nullptr != parent)
	{
		worldMatrix = localMatrix * parent->GetWorldMatrix();
	}
	else
	{
		worldMatrix = localMatrix;
	}

	forward	= DXMath::Vector3::TransformNormal(DXMath::Vector3::Forward, worldMatrix);
	right =   DXMath::Vector3::TransformNormal(DXMath::Vector3::Right, worldMatrix);
	up =      DXMath::Vector3::TransformNormal(DXMath::Vector3::Up, worldMatrix);
	look =    DXMath::Vector3::TransformNormal(DXMath::Vector3::Backward, worldMatrix);
}

DXMath::Matrix TransformComponent::GetWorldMatrix() const
{
	return worldMatrix;
}

DXMath::Matrix TransformComponent::GetLocalMatrix() const
{
	return localMatrix;
}

DXMath::Vector3 TransformComponent::GetLocalForward() const
{
	return  DXMath::Vector3::Transform(DXMath::Vector3::Forward, DXMath::Matrix::CreateFromQuaternion(rotation));
}

DXMath::Vector3 TransformComponent::GetLocalUp() const
{
	return DXMath::Vector3::Transform(DXMath::Vector3::Up, DXMath::Matrix::CreateFromQuaternion(rotation));
}

DXMath::Vector3 TransformComponent::GetLocalRight() const
{
	return DXMath::Vector3::Transform(DXMath::Vector3::Right, DXMath::Matrix::CreateFromQuaternion(rotation));
}


