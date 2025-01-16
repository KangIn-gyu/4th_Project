#include "pch.h"
#include "AiNode.h"
#include "Helper.h"

AiNode::AiNode()
{
	nodeName.reserve(20);
}

AiNode::~AiNode()
{
}

AiNode::AiNode(const AiNode& _other) : nodeName(_other.nodeName), transform(_other.transform)
{
	if (this != &_other)
	{
		child = std::move(_other.child);
		parent = _other.parent;
	}
}

AiNode::AiNode(AiNode&& _other) noexcept
{
	if (this != &_other)
	{
		nodeName = std::move(_other.nodeName);
		transform = _other.transform;
		child = std::move(_other.child);
	}
}

// 여기 상황에서는 단순히 포인터를 벡터에 추가한다. 그래서 복사가 없다
void AiNode::AddChild(AiNode* _child)
{
	child.emplace_back(_child);
}

void AiNode::SetName(std::string_view _name)
{
	nodeName = _name.data();
}

Transform AiNode::GetTransform()
{
	return transform;
}

void AiNode::ShowChild()
{
	std::cout << "<My Parent>" << '\n';
	if (nullptr == parent)
	{
		std::cout << "Null" << '\n';
	}
	else
	{
		std::cout << parent->GetName() << '\n';
	}
	
	std::cout << "< My Child >";
	if (child.empty())
	{
		std::cout << "zero" << '\n';
	}
	else
	{
		for (int i = 0; i < child.size(); i++)
		{
			std::cout << child[i]->GetName() << ", ";
		}
		std::cout << '\n';
	}	
}

void AiNode::SetLocalTransform(DX::XMMATRIX _fbxLocalTransform)
{
	transform.SetLocalMatrix(_fbxLocalTransform);
}

void AiNode::AllDelete()
{
	if (!child.empty())
	{
		for (auto& data : child)
		{
			// 자식 노드에서 AllDelete를 호출해 재귀적으로 삭제
			data->AllDelete();
		}
		child.clear(); // 자식 노드 리스트 초기화
	}

	// 자식들이 다 삭제된 후, 자신도 삭제
	SafeExtinction::SAFE_DELETE(this);
}
