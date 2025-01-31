#include "pch.h"
#include "Dealer.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "Deck.h"
#include "../Engine/Model.h"

Dealer::Dealer(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{

}

void Dealer::Initialize()
{
	CreateComponent<ModelComponent>("STAGE1/FBX/asdq2.fbx");  // char2 / gun // asdq
	CreateComponent<RenderComponent>();

	CreateComponent<BoxCollider>();
	
	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox( center, extent,GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");

}

void Dealer::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);
	
	//std::cout << GetComponent<ModelComponent>()->GetModel().get()->extent.x << std::endl;
}



void Dealer::CardDraw(Deck* _deck)
{
	while(hand.GetScore() >= maxScore)
	{
		hand.cardDraw((_deck->DrawCard(true))); //1초에한장씩등 딜레이 추가필요
	}
}

int Dealer::GetScore()
{
	return hand.GetScore();
}

void Dealer::Act()
{
	//pattern(); 한번쓰고나면 다른패턴 담아둬야함
}

void Dealer::OnClick()
{
	std::cout << "누르지 마세요 " << std::endl;
	//this->~Dealer();
}

void Dealer::OnMouse()
{
}

bool Dealer::FinishFirst()
{
	return false;
}
