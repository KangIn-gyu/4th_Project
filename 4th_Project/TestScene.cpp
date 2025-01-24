#include "pch.h"
#include "TestScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{

}

void TestScene::Enter()
{
	CreatorObject<TestObj>("char", Object::ObjectType::Basic);
	//CreatorObject<TestObj2>("gun", Object::ObjectType::Basic);
	objectManager->GetObjectss<TestObj>("char")->GetComponent<TransformComponent>()->SetPosition({50, 0, 0});

	ShowObject();
}


