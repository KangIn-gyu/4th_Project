#pragma once
#include "Transform.h"

class AiNode
{
public:
	AiNode(); // 미리 이름 설정 길어봐야 27정도 될려나?
	~AiNode();
	AiNode(const AiNode& _other);
	AiNode(AiNode&& _other) noexcept;

	void Update(const float _deltaTime);

	void AddChild(AiNode* _child);
	void SetParent(AiNode* _parent) { parent = _parent; }
	void SetName(std::string_view _name);

	std::string GetName() { return nodeName; }
	std::vector<AiNode*> GetChildren() { return child; }
	
	Transform GetTransform();
	Transform* GetPointTransform() { return &transform; }
	void ShowChild();
	void SetLocalTransform(DX::XMMATRIX _fbxLocalTransform);

	void AllDelete(); // fbx로드에서 데이터 복사용 로드 지우기용 사용하지마시오.
private:

public:

private:
	std::vector<AiNode*> child;
	AiNode* parent {};
	std::string nodeName;
	Transform transform; // 노드의 트랜스폼  
};

