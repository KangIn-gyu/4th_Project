#include "pch.h"
#include "BaseObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/TransformComponent.h"

BaseObj::BaseObj(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{

}

void BaseObj::Initialize()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/gun.fbx");  // char2 // gun
	auto* renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
	GetComponent<TransformComponent>()->SetPosition({ 50 , 0 , 0 });
}

void BaseObj::Update(const float _deltaTime)
{
}
