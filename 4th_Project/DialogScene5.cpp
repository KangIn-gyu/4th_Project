#include "pch.h"
#include "DialogScene5.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "../Engine/SceneManager.h"
#include "UIButton.h"
void DialogScene5::Enter()
{
    // TODO: 엔딩의 분기점값을 얻어야한다.
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene5", Object::ObjectType::UI,
        0, 90, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene5.csv", "여기수정해야함");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("DialogScene2");});
}

void DialogScene5::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}