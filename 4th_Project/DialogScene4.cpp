#include "pch.h"
#include "DialogScene4.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
void DialogScene4::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene4", Object::ObjectType::UI,
        0, 69, "Font/Bold.ttf", "DialogScenes/CSV/Scene4.csv", "여기수정해야함");

    dialog->CreateScript<D2DBitMapFontScript>();
}

void DialogScene4::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}