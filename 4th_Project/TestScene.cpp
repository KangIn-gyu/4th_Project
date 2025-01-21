#include "pch.h"
#include "TestScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"

TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{

}

void TestScene::Enter()
{
	CreatorObject<TestObj>("char", Object::ObjectType::Basic);
	CreatorObject<TestObj>("gun", Object::ObjectType::Basic);
	CreatorObject<TestObj2>("korea", Object::ObjectType::Basic);
	CreatorObject<TestObj2>("japan", Object::ObjectType::Basic);
	SceneLoader sceneloader("../4th_Project/123213.json");
	
	objectManager->ShowObject();
	sceneloader.InitObjs(objectManager);
	//objectManager->GetObjectss<TestObj>("char")->GetComponent<TransformComponent>()->SetPosition(DXMath::Vector3(-100,0,0));
	CreatorObject<TestObj2>("gun", Object::ObjectType::Basic);
	objectManager->GetObjectss<TestObj>("char")->GetComponent<TransformComponent>()->SetPosition({50, 0, 0});

	ShowObject();

}

