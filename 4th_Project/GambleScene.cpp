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
	//BLACKJACK->dealer = CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic); 
	//BLACKJACK->Setstage(1);
	//BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);

	
	//auto deck = objectManager->GetGameObject<Deck>(Object::ObjectType::Basic, "Deck");
	//deck->Init();
	//deck->ShuffleDeck();
	//deck->showDeck();
	
	
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3{ 600, 100, 0 }, ClickFunc::StayButton);

//	auto button1 = objectManager->GetGameObject<Button>(Object::ObjectType::Basic, "Button");
//	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3{ 800, 100, 0 }, [button1]()
//		{
//			DXMath::Vector3 currentPosition = button1->GetComponent<TransformComponent>()->GetPosition();
//
//			// x 값에 50 더하기
//			currentPosition.y += 50;
//
//			// 업데이트된 위치 설정
//			button1->GetComponent<TransformComponent>()->SetPosition(currentPosition);
//		}
//	);
	
	
}



void GambleScene::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);
	//std::cout << "겜블씬 업데이트중" << std::endl;
	//std::cout << PLAYER->GetScore() << std::endl;
	//std::cout << GAMEMANAGER->getstatestring() << std::endl;
	//std::cout << GAMEMANAGER->Getmagnification() << std::endl;
	//GAMEMANAGER->Update(_deltaTime);
}
