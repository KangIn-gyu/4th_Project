#include "pch.h"
#include "Object.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "Mesh.h"
#include "Model.h"
#include "AiNode.h"
#include "Helper.h"

ModelComponent::ModelComponent(std::string_view _filePath)
{
	model = RESOURCESYSTEM->Load<Model>(_filePath);
	model->SetOwner(this);
    model->GetModelData()->Show();
    rootNode = DeepCopyNode(model->GetModelData()->rootNode, nullptr);
}

ModelComponent::~ModelComponent()
{
    SafeExtinction::SAFE_CLEAR_CONTAINER(nodeList);
}

void ModelComponent::ComponentInitialize()
{	
	objectTransform = owner->GetComponent<TransformComponent>(0); // 오너의 트랜스폼을 넣는다.
    rootNode->GetPointTransform()->SetParent(objectTransform->GetTransform());
    model->Initialize();
}

void ModelComponent::ComponentUpdate(const float _deltaTime)
{
    rootNode->Update(_deltaTime);
}

Transform* ModelComponent::GetTransform()
{
	return objectTransform->GetTransform();
}

AiNode* ModelComponent::DeepCopyNode(AiNode* _originalNode, AiNode* _parentNode)
{
    if (!_originalNode)
        return nullptr;

    AiNode* copiedNode = new AiNode(*_originalNode);
    copiedNode->SetParent(_parentNode);

    nodeList.emplace(copiedNode->GetName(), copiedNode);
    copiedNode->GetChildren().clear();
    for (AiNode* child : _originalNode->GetChildren())
    {
        AiNode* copiedChild = DeepCopyNode(child, copiedNode);
        if (copiedChild)
        {
            copiedNode->AddChild(copiedChild);
        }
    }

    return copiedNode;
}

                                 
