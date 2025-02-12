#include "pch.h"
#include "DialogIntro.h"

#include "D2DBitMapFontScript.h" // ½ºÅ©¸³Æ®
#include "FadeEffectScript.h"

#include "../Engine/SceneManager.h"
#include "../Engine/SoundSystem.h"

#include "UIButton.h"
#include "D2DBaseObj.h"
#include "LoadingScene.h"

DialogIntro::DialogIntro(std::string_view _Name) : Scene(_Name)
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    dialog = CreatorObject<D2DBaseObj>("DialogIntro", Object::ObjectType::UI,
       0,1, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Intro.csv");
    dialog->CreateScript<D2DBitMapFontScript>();

    // 스킵버튼
    skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("LoadingScene");});

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>();


    dialog->SetD2DLayerOrder(0);
    fading->SetD2DLayerOrder(5);
    skipbutton->SetD2DLayerOrder(1);

    dialog->SetActive(false);
    fading->SetActive(false);
    skipbutton->SetActive(false);

  
}

void DialogIntro::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("LoadingScene");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}

void DialogIntro::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    fading->SetActive(true);
    skipbutton->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
    static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("DialogScene0");

    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::DialogIntro, eSoundChannel::BGM);

}
