#include "pch.h"
#include "TestScene.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"
#include "../Engine/SceneManager.h"
#include "Button.h"
#include "BlackJack.h"
#include "UIButton.h"
#include "TestObj.h"

TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	////ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//<TestObj>("Deck", Object::ObjectType::Basic);
}

void TestScene::Enter()
{
	//SceneLoader loader;
	//loader.Load("../4th_Project/2345.json");
	//loader.ImportUnityScene("../4th_Project/2345.json", this);
	//ObjectCreator<TestObj>("IngameMods", Object::ObjectType::Basic);
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(-200,0,0), []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	CreatorObject<UIButton>("bu", Object::ObjectType::UI, "STAGE1/UI/mybutton2.png", []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	CreatorObject<TestObj>("Font", Object::ObjectType::UI);
}

