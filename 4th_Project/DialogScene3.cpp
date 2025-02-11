#include "pch.h"
#include "DialogScene3.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
DialogScene3::DialogScene3(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene3", Object::ObjectType::UI,
        39, 56, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene3.csv", "GambleScene");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("GambleScene");});
}

void DialogScene3::Enter()
{

}

void DialogScene3::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogScene3::ResetInformation()
{
    dialog->SetActive(true);
    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::Scene3, eSoundChannel::BGM);
}
