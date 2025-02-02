#include "pch.h"
#include "AnimationObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"

AnimationObj::AnimationObj(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
}

void AnimationObj::Initialize()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/SkinningTest.fbx");  // char2 / gun / Robot_Dummy_classTest / SkinningTest // asdq
	auto* renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void AnimationObj::Update(const float _deltaTime)
{

}
