#include "pch.h"
#include "TestObj.h"
#include "RenderComponent.h"
#include "ModelComponent.h"

void TestObj::Start()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/char.fbx");  // char2 / gun
	CreateComponent<RenderComponent>();

	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void TestObj::Update(const float _deltaTime)
{

}
