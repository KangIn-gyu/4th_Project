#include "pch.h"
#include "DialogScene0.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h"
#include "UIButton.h"
#include "../Engine/SceneManager.h"

DialogScene0::DialogScene0(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene0", Object::ObjectType::UI,
        1, 6, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene0.csv", "DialogScene1");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),
        []() {SCENEMANAGER->ChangeScene("DialogScene1");});

    dialog->SetActive(false);
}

void DialogScene0::Enter()
{

}

void DialogScene0::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogScene0::ResetInformation()
{
    dialog->SetActive(true);
}
