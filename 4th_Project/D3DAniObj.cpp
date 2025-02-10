#include "pch.h"
#include "D3DAniObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"

D3DAniObj::D3DAniObj(std::string_view _name, ObjectType _type, std::string _filePath) : filePath(_filePath), Object(_name, _type)
{
	CreateComponent<ModelComponent>(filePath);
	auto renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void D3DAniObj::Initialize()
{
//	CreateComponent<ModelComponent>(filePath);
//	auto renderComponent = CreateComponent<RenderComponent>();
//	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
//	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void D3DAniObj::Update(const float _deltaTime)
{

}
