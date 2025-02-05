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

void BoxCollider::DrawBoundBox()
{
	D2D1_RECT_F rect = { 0, 0,  obBox.Center.x*2, obBox.Center.y * 2 };
	D2DFont* b = new D2DFont;
	D2DClass::GetD2DDeviceContext()->DrawRectangle(&rect, b->GetBoundBrush());
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
