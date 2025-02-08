#include "pch.h"
#include "DialogScene3.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
void DialogScene3::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene3", Object::ObjectType::UI,
        39, 56, "Font/Bold.ttf", "DialogScenes/CSV/Scene3csv", "여기수정해야함");

    dialog->CreateScript<D2DBitMapFontScript>();
}

void DialogScene3::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}