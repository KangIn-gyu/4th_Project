#include "pch.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "Object.h"
#include "ModelComponent.h"
#include "ResourceSystem.h"
#include "Shader.h"

RenderComponent::RenderComponent()
{
	RENDERER->AddRenderComponent(this);
}

RenderComponent::~RenderComponent()
{
	RENDERER->RemoveRenderComponent(this);
	std::cout << "RenderComponent 삭제" << std::endl;
}

void RenderComponent::ComponentInitialize()
{ 
	if (auto*modelComponet = owner->GetComponent<ModelComponent>(); nullptr != modelComponet)
	{
		modelData = modelComponet->GetModel();
		nodeData = modelComponet->GetNodeData();
	}
}

std::shared_ptr<Shader> RenderComponent::GetShder(ShaderType _shaderType)
{
	auto it = shaders.find(_shaderType);
	if (it == shaders.end())
	{ // 추후 로그 시스템 추가하자
		std::cout << "GetShder에 해당 키에 값이 없습니다." << '\n';
		return nullptr;
	}
	return it->second;
}

void RenderComponent::SetShader(ShaderType _shaderType, std::string_view _filePath)
{
	auto shader = RESOURCESYSTEM->Load<Shader>(_filePath);
	
	if (shader.get() != nullptr)
	{
		shaders[_shaderType] = shader;
	}
	else
	{
		std::cout << "SetShader에서 오류 발생" << '\n';
	}
}


