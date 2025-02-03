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

TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{

}

void TestScene::Enter()
{

	ObjectCreator<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(-200,0,0), []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	//ObjectCreator<D2DFont>("Test"), Object::ObjectType::UI);
}

