#include "pch.h"
#include "TestObj2.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"

#include "../Engine/TransformComponent.h"
#include "../Engine/BoxCollider.h"

TestObj2::TestObj2(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
}

void TestObj2::Start()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/gun.fbx");  // char2 / gun
	CreateComponent<RenderComponent>();
	
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
	CreateComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetBox({ GetComponent<TransformComponent>()->GetPosition() }, { 40,40,40 },
		GetComponent<TransformComponent>()->GetQuaternion());
}

void TestObj2::Update(const float _deltaTime)
{
	auto transformComponent = GetComponent<TransformComponent>();
	auto posi = transformComponent->GetPosition();
}
