#include "pch.h"
#include "TestObj2.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"

#include "../Engine/TransformComponent.h"
#include "../Engine/BoxCollider.h"

TestObj2::TestObj2(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
}

void TestObj2::Initialize()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/Map_test.fbx");  // char2 / gun
	auto* randerComponet = CreateComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/StaticVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
	CreateComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetBox({ GetComponent<TransformComponent>()->GetPosition() }, { 0,0,0 },
		GetComponent<TransformComponent>()->GetQuaternion());
}

void TestObj2::Update(const float _deltaTime)
{
	auto transformComponent = GetComponent<TransformComponent>();
	auto posi = transformComponent->GetPosition();
}
