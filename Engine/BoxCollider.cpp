#include "pch.h"
#include "BoxCollider.h"

#include "D2DClass.h"
#include "D2DFont.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ColliderManager.h"
#include "CircleCollider.h"
void BoxCollider::SetBox(const DXMath::Vector3 center, const DXMath::Vector3 extents, const DXMath::Quaternion orientation, ActiveType _type)
{
	obBox.Center = center;
	modelCenter = center;
	obBox.Extents = extents;
	modelExtent = extents;
	obBox.Orientation = orientation;
	colliderType = _type;
	coltype = ColliderType::Box;
	if (colliderType == ActiveType::Block)
	{
		CollidersManager->AddCollider(this);
		//블럭인것만 모으기
	}
}

bool BoxCollider::CheckCollision(Collider* _other)
{
	if (_other->coltype == ColliderType::Box) //어차피 서클에서 처리할거니까 이렇게만
	{
		BoxCollider* boxcol = dynamic_cast<BoxCollider*>(_other);
		if (_other != nullptr)
			return obBox.Intersects(boxcol->obBox);
	}
	else
	{
		CircleCollider* circle = dynamic_cast<CircleCollider*>(_other);
		if (_other != nullptr)
			return obBox.Intersects(circle->Circle);
	}

}

bool BoxCollider::Check2D(float mousex, float mousey)
{
	float minX = modelCenter.x - modelExtent.x;
	float maxX = modelCenter.x + modelExtent.x;
	float minY = modelCenter.y - modelExtent.y;
	float maxY = modelCenter.y + modelExtent.y;

	if (isDiamond)
	{
		float cx = modelCenter.x;
		float cy = modelCenter.y;
		float squareSize = maxX - minX;
		float localX = mousex - cx;
		float localY = mousey - cy;

		// 마름모 한 변의 길이 (정사각형의 대각선 길이와 관련)
		float diamondSide = squareSize / sqrt(2);

		// 마우스 좌표를 반시계 방향 45도 회전
		float rotatedX = 0.7071f * localX + 0.7071f * localY;
		float rotatedY = -0.7071f * localX + 0.7071f * localY;
		float halfSize = diamondSide / 2.0f;

		if (rotatedX >= -halfSize && rotatedX <= halfSize &&
			rotatedY >= -halfSize && rotatedY <= halfSize)
		{
			return true; // 마름모 안에 있음
		}
	}
	else
	{	// 마우스가 AABB 안에 있는지 확인
		if (mousex >= minX && mousex <= maxX &&
			mousey >= minY && mousey <= maxY)
		{
			return true;  // 충돌함
		}
	}

	return false;  // 충돌 안 함
}

D2D1_RECT_F BoxCollider::GetBoundBox()
{
	float minX = modelCenter.x - modelExtent.x;
	float maxX = modelCenter.x + modelExtent.x;
	float minY = modelCenter.y - modelExtent.y;
	float maxY = modelCenter.y + modelExtent.y;
	D2D1_RECT_F rect = { minX, minY,  maxX, maxY };

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
