#pragma once
#include "Transform.h"

class AiNode
{
public:
	AiNode() { nodeName.reserve(20); }  // 미리 이름 설정 길어봐야 27정도 될려나?
	~AiNode();
	AiNode(const AiNode& other);

	void AddChild(AiNode* _child);
	void SetParent(AiNode* _parent) { parent = _parent; }
	void SetName(std::string_view _name);

	std::string GetName() { return nodeName; }
	std::vector<AiNode*> GetChildren() { return childs; }
	Transform GetTransform() { return transform; }
	Transform* GetPointTransform() { return &transform; }
	void ShowChids();

	void AllDelete(); // fbx로드에서 데이터 복사용 로드 지우기용 사용하지마시오.
private:

public:

private:
	std::vector<AiNode*> childs;
	AiNode* parent {};
	std::string nodeName;
	Transform transform;
};

