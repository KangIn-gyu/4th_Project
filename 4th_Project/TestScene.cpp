#include "pch.h"
#include "TestScene.h"
#include "TestObj.h"
#include "TestObj2.h"
TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{

}

void TestScene::Enter()
{
	CreatorObject<TestObj>(Object::ObjectType::Basic);
	CreatorObject<TestObj2>(Object::ObjectType::Basic);
	ShowObject();
}

