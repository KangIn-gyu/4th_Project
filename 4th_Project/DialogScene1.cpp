#include "pch.h"
#include "DialogScene1.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
void DialogScene1::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
	auto* dialog = CreatorObject<D2DBaseObj>("DialogScene1", Object::ObjectType::UI,
        7,23,"Font/Bold.ttf", "DialogScenes/CSV/Scene1.csv", "여기수정해야함");

    dialog->CreateScript<D2DBitMapFontScript>();
}

void DialogScene1::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}
