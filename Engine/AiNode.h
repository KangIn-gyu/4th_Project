#pragma once
#include "Transform.h"

class AiNode
{
public:
	AiNode() { nodeName.reserve(20); }  // 미리 이름 설정 길어봐야 27정도 될려나?
	~AiNode() = default;

	void AddChild(AiNode* _child);
	void SetParent(AiNode* _parent) { parent = _parent; }
	void SetName(std::string_view _name);
	Transform GetTransform() { return transform; }
	Transform* GetPointTransform() { return &transform; }
private:

public:

private:
	std::vector<AiNode*> childs;
	AiNode* parent {};
	std::string nodeName;
	Transform transform;
};

