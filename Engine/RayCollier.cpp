#include "pch.h"
#include "RayCollier.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ColliderManager.h"
#include "BoxCollider.h"
void RayCollier::SetRay(float _rayLenth)
{
	rayLenth = _rayLenth;
	CollidersManager->AddCollider(this,ColType ::Ray);
	Trans = GetOwner()->GetComponent<TransformComponent>();
}

bool RayCollier::CheckCollision(Collider* _other)
{

	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(_other);  // BoxCollider로 캐스팅
	if (boxCollider == nullptr) {
		return false;  // 다른 콜라이더가 박스가 아니면 충돌 검사 안 함
	}

	DXMath::Vector3 rayOrigin = Trans->GetPosition();  // 레이의 시작점
	DXMath::Vector3 rayDirection = Trans->GetWorldForward();
	float distance = 0.0f;
	if (boxCollider->IntersectsRay(rayOrigin,rayDirection, distance)) {
		// 충돌이 발생하면 distance에 충돌 지점까지의 거리가 계산됨
		if(distance <= rayLenth)
			return true;  // 충돌한 경우
	}

	return false;
}



void RayCollier::ComponentInitialize()
{
}

void RayCollier::ComponentUpdate(const float _deltaTime)
{
}
