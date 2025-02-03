#include "pch.h"
#include "AiNode.h"
#include "Helper.h"
#include "Mesh.h"
#include "AnimationNode.h"

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

void AiNode::Update(const float _deltaTime, const float _progressTime)
{
	if (nullptr != animationNode)
	{
		DXMath::Vector3 position {}, scaling {};
		DXMath::Quaternion rotation {};
		animationNode->Evaluate(_progressTime, position, rotation, scaling);
		transform.SetLocalMatrix({ DirectX::SimpleMath::Matrix::CreateScale(scaling)
								 * DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation)
								 * DirectX::SimpleMath::Matrix::CreateTranslation(position) });
	}

	transform.UpdateTransform(); // Transform에 맞는 업데이트 구현 필요

	// 자식 노드들에 대해 재귀적으로 Update 호출
	for (auto* childNode : child) 
	{
		if (childNode) 
		{
			childNode->Update(_deltaTime, _progressTime);
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
		transform.SetParent(_parent->GetPtrTransform());
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

void AiNode::SetAnimationNode(AnimationNode* _aniNode)
{
	animationNode = _aniNode;
}

