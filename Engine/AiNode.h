#pragma once
#include "Transform.h"

class Mesh;
class AnimationNode;
class AiNode
{
public:
	AiNode(); // 미리 이름 설정 길어봐야 27정도 될려나?
	~AiNode();
	AiNode(const AiNode& _other);
	AiNode(AiNode&& _other) noexcept = default;
	AiNode& operator=(const AiNode& _other) = default;   // 복사 대입 연산자
	AiNode& operator=(AiNode&& _other) noexcept = default; // 이동 대입 연산자

	void Update(const float _deltaTime, const float _progressTime);

	void AddChild(AiNode* _child);
	void SetParent(AiNode* _parent);
	void SetName(std::string_view _name);
	void SetMesh(Mesh* _mesh);
	
	const std::string& GetName() { return nodeName; }
	std::vector<AiNode*>& GetChildren() { return child; }
	
	Transform GetTransform();
	Transform* GetPtrTransform() { return &transform; }
	Mesh* GetMesh();

	void ShowChild();
	void SetLocalTransform(DX::XMMATRIX _fbxLocalTransform);

	void SetAnimationNode(AnimationNode* _aniNode);
private:

public:

private:
	std::string nodeName {};
	Transform transform; // 노드의 트랜스폼  
	AiNode* parent {};

	Mesh* mesh = nullptr; // 해당 매쉬는 노드와 같은 인덱스의 매쉬이다.
	AnimationNode* animationNode {};
	std::vector<AiNode*> child;
};

