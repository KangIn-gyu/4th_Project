#include "pch.h"
#include "AiNode.h"

void AiNode::AddChild(AiNode* _child)
{
	childs.emplace_back(_child);
}

void AiNode::SetName(std::string_view _name)
{
	nodeName = _name.data();
}
