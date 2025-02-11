#include "pch.h"
#include "CircleCollider.h"
#include "TransformComponent.h"
#include "ColliderManager.h"
#include "BoxCollider.h"
#include "Object.h"
void CircleCollider::SetCircle(const DXMath::Vector3 center, float _radius, ActiveType _tpye)
{
	Circle.Center = center;
	Circle.Radius = _radius;
	radius = _radius;
	coltype = ColliderType::Circle;
	colliderType = _tpye;
	if (colliderType == ActiveType::Block)
	{
		CollidersManager->AddCollider(this);
		//블럭인것만 모으기
	}
}

bool CircleCollider::CheckCollision(Collider* _other)
{
	if(_other->coltype == ColliderType::Box) //어차피 서클에서 처리할거니까 이렇게만
	{
		BoxCollider* boxcol = dynamic_cast<BoxCollider*>(_other);
		if (_other != nullptr)
			return Circle.Intersects(boxcol->obBox);
	}
	return false;
}

void CircleCollider::ComponentInitialize()
{
}

void CircleCollider::ComponentUpdate(const float _deltaTime)
{
	auto trans = owner->GetComponent<TransformComponent>();
	DXMath::Vector3 center = trans->GetPosition();
	Circle.Center = center;

}
