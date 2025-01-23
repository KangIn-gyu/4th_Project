#include "pch.h"
#include "GambleScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/SceneManager.h"
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"
#include "Button.h"
#include "GameManager.h"

GambleScene::GambleScene(std::string_view _Name) : Scene(_Name)
{
	

	
}


void GambleScene::Enter()
{

	CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic);

	CreatorObject<Deck>("Deck", Object::ObjectType::Basic);

	
	auto deck = objectManager->GetGameObject<Deck>(Object::ObjectType::Basic, "Deck");
	deck->Init();
	deck->ShuffleDeck();
	deck->showDeck();
	
	CreatorObject<Button>("Button", Object::ObjectType::Basic, [deck]()
		{PLAYER->CardDraw(deck);});

	

}

void GambleScene::Update(const float _deltaTime)
{
	//std::cout << "°×ºí¾À ¾÷µ¥ÀÌÆ®Áß" << std::endl;
	std::cout << PLAYER->GetScore() << std::endl;
}
