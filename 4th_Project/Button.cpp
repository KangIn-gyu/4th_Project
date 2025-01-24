#include "pch.h"
#include "Button.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneManager.h"
Button::Button(std::string_view _name, Object::ObjectType _type, std::function<void(void)> _func) : Object(_name, _type)
{
	clickFunc = _func;
}

void Button::Start()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/" + name +".fbx");  //이미지 이름이랑 같게 name .fbx
	CreateComponent<RenderComponent>();

	GetComponent<TransformComponent>()->SetPosition({ 200,0,0 });  //생성할떄 저장한거 받아오게끔
	CreateComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetBox({ GetComponent<TransformComponent>()->GetPosition() }, { 40,40,40 },
		GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void Button::Update(const float _deltaTime)
{
}



void Button::OnClick()
{
	std::cout << "버튼누름" << std::endl;
	clickFunc();
}


