#include "pch.h"
#include "Object.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include "ResourceSystem.h"
#include "Mesh.h"
#include "Model.h"

ModelComponent::ModelComponent(std::wstring_view _filePath)
{
	model = RESOURCESYSTEM->Load<Model>(_filePath);
	model->SetOwner(this);
}

ModelComponent::~ModelComponent()
{
	std::cout << "ModelComponent삭제" << std::endl; // 추후 로그 시스템에 적용해야됨
}

void ModelComponent::ComponentInitialize()
{
	objectTransform = owner->GetComponent<TransformComponent>(0); // 오너의 트랜스폼을 넣는다.
	model->Initialize();
}
