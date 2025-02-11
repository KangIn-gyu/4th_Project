#include "pch.h"
#include "DialogScene4.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "../Engine/SceneManager.h"
#include "UIButton.h"
void DialogScene4::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene4", Object::ObjectType::UI,
        0, 69, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene4.csv", "GambleScene");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("GambleScene");});
}

void DialogScene4::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}