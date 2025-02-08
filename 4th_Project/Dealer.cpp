#include "pch.h"
#include "Dealer.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "Deck.h"
#include "../Engine/Model.h"
#include "../Engine/TimeSystem.h"
#include "BlackJack.h"

Dealer::Dealer(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
	auto model = CreateComponent<ModelComponent>("STAGE1/FBX/Evelyn_LowPoly.fbx"); // Evelyn char2 SkinningTest Evelyn_LowPoly
	/*if (model->GetAnimations() != nullptr)
	{
		model->SetAnimation(0);
	}*/

	CreateComponent<RenderComponent>();

	CreateComponent<BoxCollider>();
	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent * 0.6;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion());
	auto randerComponet = GetComponent<RenderComponent>();
	randerComponet->SetShader(ShaderType::VS, "Shaders/VertexShaderVS.hlsl");
	randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
}

void Dealer::Initialize()
{
	//CreateComponent<ModelComponent>("STAGE1/FBX/char2.fbx");  // char2 / gun // asdq
	

}

void Dealer::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);
	
	if (hand.numCard() >= 2)
	{
		OpenOne(_deltaTime);
		
	}
		
	//std::cout << GetComponent<ModelComponent>()->GetModel().get()->extent.x << std::endl;
}



void Dealer::Init()
{

	finishFirst = false;
	finishDraw = false;
	hand.handReset(true);
}

void Dealer::FirstDraw(Deck* _deck)
{
	
	hand.cardDraw((_deck->DrawCard(true)),{ dealerSlots.x + hand.numCard() * 5.0f, dealerSlots.y + hand.numCard() * 0.1f, dealerSlots.z},true);
	
}

void Dealer::CardDraw(Deck* _deck)
{
	static float elapsedTime = 0;
	float delta = TIMESYSTEM.get()->GetFloatDeltaTime();
	elapsedTime += delta;
	if(hand.GetScore() < 17 && elapsedTime >= 1.0f)
	{
		if(hand.hand[0]->isOpen == false)  //첫장 뒤집고
			hand.hand[0]->Open();
		else
		{
			Card* card = hand.cardDraw((_deck->DrawCard(true)),{ dealerSlots.x + hand.numCard() * 5.0f, dealerSlots.y + hand.numCard() * 0.1f, dealerSlots.z},true);
			card->MoveOpen();
		}
		elapsedTime = 0;
	}

	if (hand.GetScore() >= 17 && elapsedTime >= 3.0f)
		finishDraw = true;
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

void Dealer::ExitMouse()
{
}

void Dealer::OpenOne(float _deltaTime)
{
	//1초뒤에 뒤집어야 하나 
	if (!finishFirst)
	{
		hand.hand.back()->MoveOpen();
	}
	finishFirst = true;
}

void Dealer::Reverse()
{
	int max = BLACKJACK->player->hand.maxHand - 1;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, max);
	
	int randomSlot = distrib(gen);
	if (BLACKJACK->player->hand.hand[randomSlot] != nullptr && BLACKJACK->player->hand.hand[randomSlot]->isOpen == true)
	{
		BLACKJACK->player->hand.hand[randomSlot]->Reverse();
	}
}


