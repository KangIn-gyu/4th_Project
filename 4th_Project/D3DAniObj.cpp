#include "pch.h"
#include "D3DAniObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"

void D3DAniObj::Initialize()
{
	CreateComponent<ModelComponent>(filePath);
	auto renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void D3DAniObj::Update(const float _deltaTime)
{

}
