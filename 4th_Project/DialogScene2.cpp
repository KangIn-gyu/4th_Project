#include "pch.h"
#include "DialogScene2.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
void DialogScene2::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene2", Object::ObjectType::UI,
        22, 40, "Font/Bold.ttf", "DialogScenes/CSV/Scene2.csv", "여기수정해야함");

    dialog->CreateScript<D2DBitMapFontScript>();
}

void DialogScene2::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}