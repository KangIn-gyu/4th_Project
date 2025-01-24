#include "pch.h"
#include "TestObj2.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"

#include "../Engine/TransformComponent.h"

TestObj2::TestObj2(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
}

void TestObj2::Start()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/char.fbx");  // char2 / gun
	CreateComponent<RenderComponent>();

	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void TestObj2::Update(const float _deltaTime)
{

}
