#include "pch.h"
#include "DialogScene3.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "../Engine/SceneManager.h"
#include "UIButton.h"
void DialogScene3::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene3", Object::ObjectType::UI,
        39, 56, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene3csv", "GambleScene");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),
        []() {SCENEMANAGER->ChangeScene("GambleScene");});
}

void DialogScene3::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}