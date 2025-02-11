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
#include "../Engine/Helper.h"

Dealer::Dealer(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
	auto model = CreateComponent<ModelComponent>("Common/FBX/Evelyn.fbx"); // Evelyn char2 SkinningTest Evelyn_LowPoly
	model->SetAnimation(1);
	/*if (model->GetAnimations() != nullptr)
	{
		model->SetAnimation(0);
	}*/

	CreateComponent<RenderComponent>();

	CreateComponent<BoxCollider>();
	DXMath::Vector3 extent = GetComponent<ModelComponent>()->GetModel().get()->extent * 0.6;
	DXMath::Vector3 center = GetComponent<ModelComponent>()->GetModel().get()->center;
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion(),Type::Block);
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

bool Dealer::Act()
{
	// 여기서 한번 다이얼로그 시작하고 그거에 맞춰서 결과가 true false로 나오고 그게 false일때만 패턴 실행
	//pattern(); 한번쓰고나면 다른패턴 담아둬야함

	if (true == pattern())
	{
		SetSkill();
		return true;
	}

	return false;
}

void Dealer::OnClick()
{
	std::cout << "누르지 마세요 " << std::endl;
	//this->~Dealer();
	//SetSkill();
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

bool Dealer::reverse()
{
	std::cout << "1\n";

	std::vector<int> openSlots;
	int cardCount = BLACKJACK->player->hand.numCard();
	for (int i = 0; i < cardCount; i++) {
		if (BLACKJACK->player->hand.hand[i] != nullptr &&
			BLACKJACK->player->hand.hand[i]->isOpen) {
			openSlots.push_back(i);
		}
	}

	if (openSlots.empty()) {
		return false;
	}

//	TODO : 강인규가 수정함 25.2.9
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_int_distribution<int> distrib(0, openSlots.size() - 1);
//	int randomIndex = distrib(gen);
	int randomIndex = RandomUtil::GetRandomInt(0, openSlots.size() - 1);

	int targetSlot = openSlots[randomIndex];
	BLACKJACK->player->hand.hand[targetSlot]->Close();

	return true;
}

bool Dealer::meditation()
{
	std::cout << "2\n";
	chip *= 1.1f;
	return true;
}

bool Dealer::skillBan()
{
	std::cout << "3\n";
	BLACKJACK->player->canSkill = false;
	return true;
}

bool Dealer::slotBan()
{
	std::cout << "4\n";
	std::vector<int> activeSlots;

	int cardCount = BLACKJACK->player->hand.numCard();

	for (int i = 0; i < cardCount; i++) {
		if (BLACKJACK->player->hand.hand[i] != nullptr &&
			BLACKJACK->player->hand.hand[i]->slotActive) {
			activeSlots.push_back(i);
		}
	}

	if (activeSlots.empty()) {
		return false;
	}

//  TODO : 강인규가 수정함 랜덤유틸로 mt19937이거 생성하는 비용 줄이기 위해
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_int_distribution<int> distrib(0, activeSlots.size() - 1);
//	int randomIndex = distrib(gen);

	int randomIndex = RandomUtil::GetRandomInt(0, activeSlots.size() - 1);

	int targetSlot = activeSlots[randomIndex];
	BLACKJACK->player->hand.hand[targetSlot]->slotActive = false;
	BLACKJACK->player->hand.hand[targetSlot]->AddEffect(Object::Effect::Banned);
	
	return true;
}

void Dealer::SetSkill()
{
	static const DSkill allSkills[] = { DSkill::reverse, DSkill::meditation, DSkill::skillBan, DSkill::slotBan };

//  TODO : 25.2.9  강인규가 수정함
//	static const size_t skillCount = sizeof(allSkills) / sizeof(DSkill);
//	std::random_device rd;
//	std::mt19937 gen(rd());


	DSkill selectedSkill;
	do {
		int randomIndex = RandomUtil::GetRandomInt(1, 4);
		selectedSkill = allSkills[randomIndex];
	} while (selectedSkill == previousSkill || selectedSkill == DSkill::none);

	previousSkill = selectedSkill;

	if (previousSkill == DSkill::reverse)
	{
		turnCount = 3;
		pattern = [this]() { return reverse(); };
	}
	else if (previousSkill == DSkill::meditation)
	{
		turnCount = 4;
		pattern = [this]() { return meditation();};
	}
	else if (previousSkill == DSkill::skillBan) 
	{
		turnCount = 2;
		pattern = [this]() { return skillBan(); };
	}
		
	else if (previousSkill == DSkill::slotBan)
	{
		turnCount = 3;
		pattern = [this]() { return slotBan(); };
	}
}

void Dealer::CardAnimation()
{

}


