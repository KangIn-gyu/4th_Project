#include "pch.h"
#include "Button.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Model.h"

Button::Button(std::string_view _name, Object::ObjectType _type, std::function<void(void)> _func) : Object(_name, _type)
{
	clickFunc = _func;
	


	GetComponent<TransformComponent>()->SetPosition({ 20000,0,0 });  //생성할떄 저장한거 받아오게끔
}

Button::Button(std::string_view _name, Object::ObjectType _type, DXMath::Vector3 _pos, std::function<void(void)> _func) : Object(_name, _type)
{
	clickFunc = _func;
	
	CreateComponent<ModelComponent>("STAGE1/FBX/" + GetName() + ".fbx");
	CreateComponent<RenderComponent>();
	CreateComponent<BoxCollider>();
	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");

	GetComponent<TransformComponent>()->SetPosition(_pos);  
}

void Button::Initialize()
{
	/*CreateComponent<ModelComponent>("STAGE1/FBX/" + GetName() + ".fbx");
	CreateComponent<RenderComponent>();
	CreateComponent<BoxCollider>();
	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");*/
}

void Button::Update(const float _deltaTime)
{
}



void Button::SetOnclick(std::function<void(void)> _func)
{
	clickFunc = _func;
}

void Button::OnClick()
{
	std::cout << "버튼 누름" << std::endl;
	clickFunc();
}


