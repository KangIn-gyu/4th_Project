#include "pch.h"
#include "TestObj.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"

TestObj::TestObj(Object::ObjectType _type) : Object(_type)
{
}

void TestObj::Start()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/char2.fbx");  // char2 / gun
	CreateComponent<RenderComponent>();

	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}
