#include "pch.h"
#include "BoxCollider.h"

#include "D2DClass.h"
#include "D2DFont.h"
#include "Object.h"
#include "TransformComponent.h"

void BoxCollider::SetBox(const DXMath::Vector3 center, const DXMath::Vector3 extents, const DXMath::Quaternion orientation)
{
	obBox.Center = center;
	modelCenter = center; 
	obBox.Extents = extents;
	modelExtent = extents;
	obBox.Orientation = orientation;
}

bool BoxCollider::Check2D(float mousex, float mousey)
{
	float minX = modelCenter.x - modelExtent.x;
	float maxX = modelCenter.x + modelExtent.x;
	float minY = modelCenter.y - modelExtent.y;
	float maxY = modelCenter.y + modelExtent.y;

	// 마우스가 AABB 안에 있는지 확인
	if (mousex >= minX && mousex <= maxX &&
		mousey >= minY && mousey <= maxY)
	{
		return true;  // 충돌함
	}

	return false;  // 충돌 안 함
}

D2D1_RECT_F BoxCollider::GetBoundBox()
{
	float r = obBox.Center.x * 2;
	D2D1_RECT_F rect = { 0, 0,  r, obBox.Center.y * 2 };
	return rect;
}

void BoxCollider::ComponentInitialize()
{
}

void BoxCollider::ComponentUpdate(const float _deltaTime)
{
	
	auto trans = owner->GetComponent<TransformComponent>();
	objScale = trans->GetScale();
	DXMath::Vector3 realExtent;
	realExtent = modelExtent * objScale;
	DXMath::Vector3 center = trans->GetPosition() + modelCenter;
	obBox.Center = center;
	obBox.Orientation = trans->GetQuaternion();
}

bool BoxCollider::IntersectsRay(const DXMath::Vector3& rayOrigin, const DXMath::Vector3& rayDirection, float& distance) const
{
	DirectX::XMVECTOR origin = DirectX::XMLoadFloat3(&rayOrigin);
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&rayDirection);

	return obBox.Intersects(origin, direction, distance);
}
