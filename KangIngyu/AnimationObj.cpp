#include "pch.h"
#include "AnimationObj.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/AiNode.h"

AnimationObj::AnimationObj(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
}

void AnimationObj::Initialize()
{
	auto* model = CreateComponent<ModelComponent>("STAGE1/FBX/SkinningTest.fbx");  // char2 / gun / Robot_Dummy_classTest / SkinningTest // asdq // Lashushya // Evelyn // Test
//	model->SetAnimation(4);
	auto* renderComponent = CreateComponent<RenderComponent>();
	renderComponent->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	renderComponent->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
	GetComponent<TransformComponent>()->SetPosition({ 50 , 0 , 0 });
}

void AnimationObj::Update(const float _deltaTime)
{

}

void AnimationObj::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	auto* modelComponent = GetComponent<ModelComponent>();
	auto* tr = GetComponent<TransformComponent>();

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D1))
	{
		modelComponent->SetAnimation(0);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D2))
	{
		modelComponent->SetAnimation(1);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D3))
	{
		modelComponent->SetAnimation(2);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D4))
	{
		modelComponent->SetAnimation(3);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D5))
	{
		modelComponent->SetAnimation(4);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D6))
	{
		modelComponent->SetAnimation(5);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D7))
	{
		modelComponent->SetAnimation(8);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D8))
	{
		modelComponent->SetAnimation(9);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D9))
	{
		modelComponent->SetAnimation(10);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D0))
	{
		modelComponent->SetAnimation(11);
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::O))
	{
		tr->SetPosition({ 0, 0, 0 });
	}

	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::P))
	{
		tr->SetPosition({ 100, 0, 0 });
	}
}
