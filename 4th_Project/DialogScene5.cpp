#include "pch.h"
#include "DialogScene5.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
void DialogScene5::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene5", Object::ObjectType::UI,
        0, 90, "Font/Bold.ttf", "DialogScenes/CSV/Scene5.csv", "여기수정해야함");

    dialog->CreateScript<D2DBitMapFontScript>();
}

void DialogScene5::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}