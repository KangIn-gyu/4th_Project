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
	CreateComponent<ModelComponent>("STAGE1/FBX/gun.fbx");  // char2 / gun
	CreateComponent<RenderComponent>();

	boxMax = { 40,40 };
	boxMin = { 40,40 };
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");


}

void TestObj2::Update(const float _deltaTime)
{
	auto transformComponent = GetComponent<TransformComponent>();
	auto posi = transformComponent->GetPosition();
	boxMin = { posi.x / 2.0f, posi.y - 40 / 2.0f};
	boxMax = { posi.x + 40 / 2.0f, posi.y + 40 / 2.0f };
}
