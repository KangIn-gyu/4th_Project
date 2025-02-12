#include "pch.h"
#include "BadEnding.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "FadeEffectScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
BadEnding::BadEnding(std::string_view _Name) : Scene(_Name)
{
    // TODO: 엔딩의 분기점값을 얻어야한다.

    dialog = CreatorObject<D2DBaseObj>("BadEnding", Object::ObjectType::UI,
        0, 98, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/BadEnding.csv");

    dialog->CreateScript<D2DBitMapFontScript>();
    skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("GambleScene");});

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>()->StartFadeOut();

    dialog->SetD2DLayerOrder(0);
    fading->SetD2DLayerOrder(5);
    skipbutton->SetD2DLayerOrder(1);

    dialog->SetActive(false);
    fading->SetActive(false);
    skipbutton->SetActive(false);
}

void BadEnding::Enter()
{
    // TODO: 엔딩의 분기점값을 얻어야한다.
}

void BadEnding::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("GambleScene");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}


void BadEnding::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    fading->SetActive(true);
    skipbutton->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();

    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::BadEnding, eSoundChannel::BGM);
}

