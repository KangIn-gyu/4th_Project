#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{

public:
	CircleCollider() = default;

	~CircleCollider() = default;

	void SetCircle(const DXMath::Vector3 center, float radius, ActiveType _tpye = ActiveType::None);

	virtual bool CheckCollision(Collider* _other)override;

	virtual void ComponentInitialize() override;                    // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override;  // 업데이트

	Touch TouchType = Touch::Basic;
	ActiveType colliderType = ActiveType::None;
	DirectX::BoundingSphere Circle;
private:
	float radius = 0;

};

