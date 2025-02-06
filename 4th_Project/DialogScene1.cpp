#include "pch.h"
#include "DialogScene1.h"
#include "D2DBaseObj.h"

void DialogScene1::Enter()
{
	auto* dialog = CreatorObject<D2DBaseObj>("Dialog", Object::ObjectType::UI, "Font/DialogScene.ttf");
}

void DialogScene1::Update(const float _deltaTime)
{

}
