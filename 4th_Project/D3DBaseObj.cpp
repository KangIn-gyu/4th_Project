#include "pch.h"
#include "D3DBaseObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"
#include "D3DObjScript.h"

D3DBaseObj::D3DBaseObj(std::string_view _name, ObjectType _type, std::string _filePath) : filePath(_filePath), Object(_name, _type)
{
	CreateComponent<ModelComponent>(filePath);
	CreateScript<D3DObjScript>();
	auto renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/StaticVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void D3DBaseObj::Initialize()
{ // 여기서는 컴포넌트 만들기만 하고
	
}

void D3DBaseObj::Update(const float _deltaTime)
{

}