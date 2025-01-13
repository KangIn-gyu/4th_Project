#include "pch.h"
#include "TestScene.h"
#include "TestObj.h"

TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{

}

void TestScene::Enter()
{
	CreatorObject<TestObj>(Object::ObjectType::Basic);
	ShowObject();
}

