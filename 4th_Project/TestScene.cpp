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
	
	
	
}

void TestScene::Enter()
{

	//SceneLoader loader;
	//loader.Load("../4th_Project/2345.json");
	//loader.ImportUnityScene("../4th_Project/2345.json", this);
	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(-200,0,0), []() {SCENEMANAGER->ChangeScene("GAMBLE");});

}

