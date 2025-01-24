#pragma once
#include "Component.h"

class TransformComponent;
class Mesh;
class Model;
class Transform;
class AiNode;
class ModelComponent : public Component
{
public:
	ModelComponent(std::string_view _filePath);
	virtual ~ModelComponent();

	virtual void ComponentInitialize() override;                       // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) override;	   // 업데이트

	std::unordered_map<std::string, AiNode*>* GetNodeData() { return &nodeList; }
	std::shared_ptr<Model> GetModel() { return model; }

	Transform* GetTransform();
	AiNode* GetRootNode() { return rootNode; }

private:
	AiNode* DeepCopyNode(AiNode* _originalNode, AiNode* _parentNode);

public:

private:
	TransformComponent* objectTransform {}; // 오너의 트랜스폼.
	std::shared_ptr<Model> model;

	std::unordered_map<std::string ,AiNode*> nodeList;
	AiNode* rootNode;
};

