#include "pch.h"
#include "TestScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/SceneManager.h"
#include "Deck.h"
#include "Button.h"

#include "Dealer.h"
#include "TestObj2.h"
TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{

}

void TestScene::Enter()
{
//	CreatorObject<Button>("Button", Object::ObjectType::Basic, []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	CreatorObject<Dealer>("TestChar", Object::ObjectType::Basic);
//	CreatorObject<TestObj>("Test1", Object::ObjectType::Basic);
//	CreatorObject<TestObj2>("Test2", Object::ObjectType::Basic);
}

