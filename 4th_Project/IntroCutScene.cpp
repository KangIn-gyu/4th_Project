#include "pch.h"
#include "IntroCutScene.h"
#include "D3DBaseObj.h"
#include "TestObj.h"

void IntroCutScene::Enter()
{
	ObjectCreator<D3DBaseObj>("TEST", Object::ObjectType::Basic, "STAGE1/FBX/Table.fbx");
	ObjectCreator<TestObj>("Font", Object::ObjectType::UI);
}

