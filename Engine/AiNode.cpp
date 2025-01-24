#include "pch.h"
#include "AiNode.h"
#include "Helper.h"
#include "Mesh.h"

AiNode::AiNode()
{
	nodeName.reserve(20);
}

AiNode::~AiNode()
{
}

AiNode::AiNode(const AiNode& _other)
	: nodeName(_other.nodeName), transform(_other.transform), mesh(_other.mesh), parent(_other.parent)
{
}

//	AiNode::AiNode(const AiNode& _other) : nodeName(_other.nodeName), transform(_other.transform), mesh(_other.mesh)
//	{ // 이거 변경하면서 문제 생김
//		for (auto& data: _other.child)
//		{
//			auto* childData = new AiNode(*data);
//			childData->SetParent(this);
//			this->child.push_back(childData);
//			childData->transform.SetParent(&this->transform);
//		}
//	}
//	
//	AiNode::AiNode(AiNode&& _other) noexcept
//	{
//		if (this != &_other)
//		{
//			nodeName = std::move(_other.nodeName);
//			transform = _other.transform;
//			child = std::move(_other.child);
//			parent = nullptr;
//		}
//	}

//	AiNode& AiNode::operator=(const AiNode& _other)
//	{
//		if (this != &_other) 
//		{
//			nodeName = _other.nodeName;
//			transform = _other.transform;
//	
//			// 기존 자원을 해제
//			for (auto& data : child) 
//			{
//				delete data;
//			}
//			child.clear();
//	
//			// 깊은 복사 수행
//			for (auto& data : _other.child) 
//			{
//				child.push_back(new AiNode(*data));
//			}
//	
//			parent = _other.parent;
//			mesh = _other.mesh;
//		}
//		return *this;
//	}
//	
//	AiNode& AiNode::operator=(AiNode&& _other) noexcept
//	{
//		if (this != &_other) 
//		{
//			// 기존 자원을 해제
//			for (auto& data : child) 
//			{
//				delete data;
//			}
//			child.clear();
//	
//			nodeName = std::move(_other.nodeName);
//			transform = _other.transform;
//			child = std::move(_other.child);
//			parent = _other.parent;
//			mesh = _other.mesh;
//	
//			// 이동된 객체의 자원을 초기화
//			_other.parent = nullptr;
//			_other.mesh = nullptr;
//		}
//		return *this;
//	}

void AiNode::Update(const float _deltaTime)
{
	transform.UpdateTransform(); // Transform에 맞는 업데이트 구현 필요

	// 자식 노드들에 대해 재귀적으로 Update 호출
	for (auto* childNode : child) 
	{
		if (childNode) 
		{
			childNode->Update(_deltaTime);
		}
	}
}

// 여기 상황에서는 단순히 포인터를 벡터에 추가한다. 그래서 복사가 없다
void AiNode::AddChild(AiNode* _child)
{
	child.emplace_back(_child);
}

void AiNode::SetParent(AiNode* _parent)
{
	if (nullptr != _parent)
	{
		parent = _parent;
		transform.SetParent(_parent->GetPointTransform());
	}
}

void AiNode::SetName(std::string_view _name)
{
	nodeName = _name.data();
}

void AiNode::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;
}

Transform AiNode::GetTransform()
{
	return transform;
}

Mesh* AiNode::GetMesh()
{
	return mesh;
}

void AiNode::ShowChild()
{
	std::cout << "<My Node Parent>" << '\n';
	if (nullptr == parent)
	{
		std::cout << "Null" << '\n';
	}
	else
	{
		std::cout << parent->GetName() << '\n';
	}
	
	std::cout << "< My Node Child >";
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

//void AiNode::AllDelete()
//{
//	if (!child.empty())
//	{
//		for (auto& data : child)
//		{
//			// 자식 노드에서 AllDelete를 호출해 재귀적으로 삭제
//			data->AllDelete();
//		}
//		child.clear(); // 자식 노드 리스트 초기화
//	}
//
//	// 자식들이 다 삭제된 후, 자신도 삭제
//	SafeExtinction::SAFE_DELETE(this);
//}
