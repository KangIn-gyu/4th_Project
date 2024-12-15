#include "pch.h"
#include "Object.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

ModelComponent::ModelComponent()
{
	
}

ModelComponent::~ModelComponent()
{

}

void ModelComponent::ComponentInitialize()
{
	auto transformComponent = owner->GetComponent<TransformComponent>(0);
}
