#include "pch.h"
#include "DialogIntro.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // ½ºÅ©¸³Æ®
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"

DialogIntro::DialogIntro(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogIntro", Object::ObjectType::UI,
        7, 23, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Intro.csv", "DialogScene0");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),
        []() {SCENEMANAGER->ChangeScene("DialogScene0");});

    dialog->SetActive(false);
}

void DialogIntro::Enter()
{

}

void DialogIntro::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogIntro::ResetInformation()
{
    dialog->SetActive(true);
}
