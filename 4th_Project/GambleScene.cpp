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
	
	GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetPosition({ -30.0f, 130.0f, -83.0f });
	GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetQuaternion(DXMath::Quaternion::Quaternion(0.3f, 0.171f, -0.059f, 0.93f));
	BLACKJACK->dealer = ObjectCreator<Dealer>("Dealer", Object::ObjectType::Basic); 
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 0, -13.0f, 50.0f });
	BLACKJACK->deck = ObjectCreator<Deck>("Deck", Object::ObjectType::Basic);
	BLACKJACK->deck->GetComponent<TransformComponent>()->SetPosition({ -60, 65, 0 });
	BLACKJACK->trashDeck = ObjectCreator<Deck>("Deck", Object::ObjectType::Basic,false);
	BLACKJACK->Setstage(1);

	//auto test = ObjectCreator<TestObj>("Table", Object::ObjectType::Basic);
	auto deck = GetGameObject(Object::ObjectType::Basic, "Deck");

	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(1100, 200, 0), []() {BLACKJACK->Bet();});
	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(900, 0, 0), []() {ClickFunc::OpenButton();});	
	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(1100, -200, 0), []() {ClickFunc::HitButton();});
	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(1000, -400, 0), []() {ClickFunc::StayButton(); });
}



void GambleScene::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);

	BLACKJACK->Update(_deltaTime);
	PLAYER->Update(_deltaTime);
}
