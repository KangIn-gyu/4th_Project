#pragma once
#include "Component.h"

class TransformComponent;
class Mesh;
class Model;
class ModelComponent : public Component
{
public:
	ModelComponent() = default;
	virtual ~ModelComponent() = default;

	virtual void ComponentInitialize() override;                       // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override {};  // 업데이트
	virtual void ComponentRender() override {};					       // 렌더용

	void Load(std::wstring_view _filePath);
private:

public:

private:
	TransformComponent* objectTransform {}; // 오너의 트랜스폼.
	std::shared_ptr<Model> model;
};

