#include "pch.h"
#include "DialogScene2.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "UIButton.h"
#include "../Engine/SceneManager.h"
#include "../Engine/SoundSystem.h"
DialogScene2::DialogScene2(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene2", Object::ObjectType::UI,
        22, 40, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene2.csv", "LobbyScene");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI, 
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("LobbyScene");});
    dialog->SetActive(false);
}

void DialogScene2::Enter()
{

}

void DialogScene2::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogScene2::ResetInformation()
{
    dialog->SetActive(true);
    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::Scene2, eSoundChannel::BGM);
}
