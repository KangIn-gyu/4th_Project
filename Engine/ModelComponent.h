#pragma once
#include "Component.h"
#include "ConstantBufferData.h"

class TransformComponent;
class Mesh;
class Model;
class Transform;
class AiNode;
class Animation;
class SkeletonInfo;
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

	void SetAnimation(int _index);
	std::vector<Animation*>* GetAnimations() { return modelAnimation; }
	int GetActiveAnimationIndex();
	void StopAnimation();

private:
	AiNode* DeepCopyNode(AiNode* _originalNode, AiNode* _parentNode);

public:

private:
	TransformComponent* objectTransform {}; // 오너의 트랜스폼.
	std::shared_ptr<Model> model;

	AiNode* rootNode;
	std::unordered_map<std::string ,AiNode*> nodeList;

	std::vector<Animation*>* modelAnimation {};
	Animation* activeAnimation = nullptr;
	float progressAnimTime{};

	// 본처리
	SkeletonInfo* skeletonInfo;
	MatrixPallete matrixPalletBuffer;
};
// 모델 컴포넌트에 애니메이션까지 포함해서 처리하게 했다
