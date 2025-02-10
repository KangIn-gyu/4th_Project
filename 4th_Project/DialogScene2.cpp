#include "pch.h"
#include "DialogScene2.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "UIButton.h"
#include "../Engine/SceneManager.h"
void DialogScene2::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene2", Object::ObjectType::UI,
        22, 40, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene2.csv", "LobbyScene");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI, 
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("LobbyScene");});
}

void DialogScene2::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}