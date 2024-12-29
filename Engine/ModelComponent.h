#pragma once
#include "Component.h"

class TransformComponent;
class Mesh;
class ModelComponent : public Component
{
public:
	ModelComponent();
	virtual ~ModelComponent();

	virtual void ComponentInitialize() override;                       // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override {};  // 업데이트
	virtual void ComponentRender() override {};					       // 렌더용

private:

public:

private:
	TransformComponent* objectTransform; // 오너의 트랜스폼.
	Mesh* mesh;   // 일단 테스트용
};

