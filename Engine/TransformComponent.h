#pragma once
#include "Component.h"
class TransformComponent : public Component
{
public:
	TransformComponent() : position(DXMath::Vector3::Zero), rotation(DXMath::Quaternion::Identity), scale(DXMath::Vector3::One){}
	virtual ~TransformComponent() { parent = nullptr; }

	virtual void ComponentInitialize() override {}
	// ComponentUpdate에 업데이트 메트릭스 계산 안넣은 이유 가만히 있을때 계산안하기 위해서
	virtual void ComponentUpdate(float _deltaTime) override {} 
	virtual void ComponentRender() override {}

	void UpdateTransform();  

	DXMath::Matrix  GetWorldMatrix() const;
	DXMath::Matrix  GetLocalMatrix() const;

	// 로컬 회전만 고려한 방향 계산
	DXMath::Vector3 GetLocalForward() const;
	DXMath::Vector3 GetLocalUp()	  const;
	DXMath::Vector3 GetLocalRight()	  const;

	// 부모 트랜스폼의 영향을 받은 계산
	DXMath::Vector3 GetWorldForward() const { return forward; }
	DXMath::Vector3 GetWorldRight() const { return right; }
	DXMath::Vector3 GetWorldUp() const { return up; }
	DXMath::Vector3 GetWorldLook() const { return look; }

	void SetParent(TransformComponent* _parent) { parent = _parent; }
private:

public:

private:
	DXMath::Vector3     position;   // 위치
	DXMath::Quaternion  rotation;   // 회전 (쿼터니언)
	DXMath::Vector3     scale;      // 스케일
	TransformComponent* parent {};

	DXMath::Vector3     forward {};
	DXMath::Vector3     right   {};
	DXMath::Vector3     up	    {};
	DXMath::Vector3     look    {};

	DXMath::Matrix		localMatrix = DXMath::Matrix::Identity;
	DXMath::Matrix		worldMatrix = DXMath::Matrix::Identity;
};

