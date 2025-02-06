#include "pch.h"
#include "TestObj.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/D2DRenderComponent.h"


TestObj::TestObj(std::string_view _name, Object::ObjectType _type) : Object(_name , _type)
{
}

void TestObj::Initialize()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/table.fbx");  // char2 / gun

	CreateComponent<RenderComponent>();

	CreateComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetBox({ GetComponent<TransformComponent>()->GetPosition() }, { 0,0,0 },
		GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/StaticVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
	SetEffect(Object::Effect::OutLine);
	//auto D2DComponent = CreateComponent<D2DRenderComponent>();
	/*D2DComponent->LoadFont("Font/ABCD.ttf");
	D2DComponent->Load2DImage("STAGE1/UI/mybutton.png");
	D2DComponent->Set2DImagePos(0, 0);
	D2DComponent->SetFontPos(100, 100);
	D2DComponent->SetDialog(L"gdgdgdgdg안녕하세요");*/
}

void TestObj::OnClick()
{
	std::cout << "1번 캐릭터" << std::endl;
	
	//std::cout << "1번 캐릭터 눌렀스빈다" << std::endl;
}

void TestObj::OnMouse()
{
	//std::cout << "1번 캐릭터 위에 마우스가 있씁니다" << std::endl;
}
