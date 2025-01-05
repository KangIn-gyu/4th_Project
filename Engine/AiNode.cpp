#include "pch.h"
#include "AiNode.h"
#include "Helper.h"

AiNode::~AiNode()
{

}

AiNode::AiNode(const AiNode& other) : nodeName(other.nodeName), transform(other.transform)
{

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

void AiNode::ShowChids()
{
	std::cout << "My Chides : ";
	if (childs.empty()) 
	{
		std::cout << "zero" << '\n';
	}
	else
	{
		for (int i = 0; i < childs.size(); i++)
		{
			std::cout << childs[i]->GetName() << ", ";
		}
		std::cout << '\n';
	}	
}

void AiNode::AllDelete()
{
	if (!childs.empty())
	{
		for (auto& data : childs)
		{
			// 자식 노드에서 AllDelete를 호출해 재귀적으로 삭제
			data->AllDelete();
		}
		childs.clear(); // 자식 노드 리스트 초기화
	}

	// 자식들이 다 삭제된 후, 자신도 삭제
	SafeExtinction::SAFE_DELETE(this);
}
