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
	std::shared_ptr<TransformComponent> objectTransform;
};

