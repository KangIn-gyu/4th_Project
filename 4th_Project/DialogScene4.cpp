#include "pch.h"
#include "DialogScene4.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "../Engine/SceneManager.h"
#include "UIButton.h"
DialogScene4::DialogScene4(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene4", Object::ObjectType::UI,
        0, 69, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene4.csv", "GambleScene");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),
        []() {SCENEMANAGER->ChangeScene("GambleScene");});

    dialog->SetActive(false);
}

void DialogScene4::Enter()
{

}

void DialogScene4::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogScene4::ResetInformation()
{
    dialog->SetActive(true);
}
