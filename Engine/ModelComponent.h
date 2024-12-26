#pragma once
#include "Component.h"

class TransformComponent;
class ModelComponent : public Component
{
public:
	ModelComponent();
	virtual ~ModelComponent();

	virtual void ComponentInitialize() override;                 // 초기화용
	virtual void ComponentUpdate(float _deltaTime) override {};  // 업데이트
	virtual void ComponentRender() override {};					 // 렌더용
private:

public:

private:
	std::shared_ptr<TransformComponent> objectTransform; // 나중에 수정할 수도 있음 원시로 해도 문제 없을 거 같기도 함
};

