#include "pch.h"
#include "D3DBaseObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"
#include "D3DObjScript.h"

void D3DBaseObj::Initialize()
{ // 여기서는 컴포넌트 만들기만 하고
	CreateComponent<ModelComponent>(filePath);
	CreateScript<D3DObjScript>();
	auto renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void D3DBaseObj::Update(const float _deltaTime)
{

}
