#pragma once
#include "Collider.h"

class TransformComponent;
class RayCollier : public Collider
{

public:
	RayCollier() = default;
	~RayCollier() = default;

	//레이 사정거리
	void SetRay(float _rayLenth);

	
	virtual bool CheckCollision(Collider* _other) override;


	virtual void ComponentInitialize() override;                    // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override;  // 업데이트

private:
	TransformComponent* Trans;
	DirectX::XMVECTOR rayOrigin;
	DirectX::XMVECTOR rayDirection;
	float rayLenth;
};

