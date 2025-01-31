#include "pch.h"
#include "GambleScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
//#include "../Engine/SceneLoader.h"
#include "../Engine/SceneManager.h"
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"
#include "Button.h"
#include "BlackJack.h"
#include "ClickFunc.h"

GambleScene::GambleScene(std::string_view _Name) : Scene(_Name)
{
	
	BLACKJACK->player = PLAYER;
	
	
}


void GambleScene::Enter()
{
	//딜러도 전역으로 할까요

	BLACKJACK->dealer = CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic); 
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ -300, 0, 0 });
	BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);
	BLACKJACK->Setstage(1);

	
	auto deck = GetGameObject(Object::ObjectType::Basic, "Deck");
	
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(900, 0, 0), []() {ClickFunc::OpenButton();});
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(1000, 0, 0), []() {ClickFunc::StayButton();});
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(1100, 0, 0), []() {ClickFunc::HitButton();});
}



void GambleScene::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);

	BLACKJACK->Update(_deltaTime);
}
