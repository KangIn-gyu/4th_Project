#include "pch.h"
#include "AiNode.h"
#include "Helper.h"

AiNode::~AiNode()
{
	if (false == childs.empty())
	{
		for (auto& data : childs)
		{
			SafeExtinction::SAFE_DELETE(data);
		}
	}
}

// 여기 상황에서는 단순히 포인터를 벡터에 추가한다. 그래서 복사가 없다
void AiNode::AddChild(AiNode* _child)
{
	childs.emplace_back(_child);
}

void AiNode::SetName(std::string_view _name)
{
	nodeName = _name.data();
}
