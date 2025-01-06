#pragma once
#include "Component.h"

class TransformComponent;
class Mesh;
class Model;
class ModelComponent : public Component
{
public:
	ModelComponent(std::string_view _filePath);
	virtual ~ModelComponent();

	virtual void ComponentInitialize() override;                       // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override;	   // 업데이트


	std::shared_ptr<Model> GetModel() { return model; }

private:

public:

private:
	TransformComponent* objectTransform {}; // 오너의 트랜스폼.
	std::shared_ptr<Model> model;
};

