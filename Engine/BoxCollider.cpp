#include "pch.h"
#include "BoxCollider.h"

void BoxCollider::SetBox(const DXMath::Vector3& center, const DXMath::Vector3& extents, const DXMath::Quaternion& orientation)
{
	obBox.Center = center;
	obBox.Extents = extents;
	obBox.Orientation = orientation;
}

void BoxCollider::ComponentInitialize()
{
}

void BoxCollider::ComponentUpdate(const float _deltaTime)
{
}

void BoxCollider::Update(const DXMath::Vector3& center, const DXMath::Quaternion& orientation)
{
	obBox.Center = center;
	obBox.Orientation = orientation;
}

bool BoxCollider::IntersectsRay(const DXMath::Vector3& rayOrigin, const DXMath::Vector3& rayDirection, float& distance) const
{
	DirectX::XMVECTOR origin = DirectX::XMLoadFloat3(&rayOrigin);
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&rayDirection);

	return obBox.Intersects(origin, direction, distance);
}
