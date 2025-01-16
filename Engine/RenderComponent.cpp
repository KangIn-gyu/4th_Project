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
{ // 음.. 고민 사항. 너무 커플링된 구조이고 꼭 모델이 아니여도 UI도 받아야 하는 상황이 올텐데 좀더 고민을 하자
	modelData = owner->GetComponent<ModelComponent>()->GetModel();
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
	shaders[_shaderType] = RESOURCESYSTEM->Load<Shader>(_filePath);
}


