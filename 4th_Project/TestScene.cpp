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
	CreatorObject<TestObj2>("gun", Object::ObjectType::Basic);
	SceneLoader sceneloader("../4th_Project/SceneData.txt");
	objectManager->GetObjectss<TestObj>("char")->GetComponent<TransformComponent>()->SetPosition(sceneloader.scenedata.objDatas[0].position);
	//objectManager->GetObjectss<TestObj>("char")->GetComponent<TransformComponent>()->SetPosition(DXMath::Vector3(-100,0,0));
	ShowObject();
	//json j;
}
