#include "pch.h"
#include "Object.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "Mesh.h"
#include "Model.h"

ModelComponent::ModelComponent(std::string_view _filePath)
{
	model = RESOURCESYSTEM->Load<Model>(_filePath);
	model->SetOwner(this);
}

ModelComponent::~ModelComponent()
{
	std::cout << "ModelComponent 삭제" << std::endl;
}

void ModelComponent::ComponentInitialize()
{
	objectTransform = owner->GetComponent<TransformComponent>(0); // 오너의 트랜스폼을 넣는다.
	model->Initialize();
//	model->GetModelData()->rootNode->Set
}

void ModelComponent::ComponentUpdate(const float _deltaTime)
{
	model->Update(_deltaTime);
}
