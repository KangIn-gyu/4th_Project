#include "pch.h"
#include "TestScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"
#include "../Engine/SceneManager.h"
#include "Deck.h"
#include "Button.h"
#include "BlackJack.h"
#include "Card.h"


TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{
	//ObjectCreator<Dealer>("Evelyn", Object::ObjectType::Basic);
	/*ObjectCreator<Card>("Card_Clover_02", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Clover_King", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Diamond_02", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Diamond_05", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Diamond_King", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Heart_03", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Heart_King", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Pack", Object::ObjectType::Basic);
	ObjectCreator<Card>("Card_Spades_King", Object::ObjectType::Basic);
	ObjectCreator<Card>("Blackjack_Table", Object::ObjectType::Basic);*/
	ObjectCreator<TestObj>("1234", Object::ObjectType::Basic);
	
}

void TestScene::Enter()
{
	//auto test = ObjectCreator<Card>("Cube", Object::ObjectType::Basic);
	//test->GetComponent<TransformComponent>()->SetPosition({ 0,-100,0 });
	//auto test2 = ObjectCreator<Card>("Cube", Object::ObjectType::Basic);
	//test2->GetComponent<TransformComponent>()->SetPosition({ 
	// 
	// 0,-100,0 });
	//SceneLoader loader;
	//loader.Load("../4th_Project/2345.json");
	//loader.ImportUnityScene("../4th_Project/2345.json", this);
	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(-200,0,0), []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	//ObjectCreator<D2DFont>("Test"), Object::ObjectType::UI);


}

