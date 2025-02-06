#include "pch.h"
#include "TestObj.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"



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
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");


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
