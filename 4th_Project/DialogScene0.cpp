#include "pch.h"
#include "DialogScene0.h"
#include "D2DBaseObj.h"

#include "D2DBitMapFontScript.h"
#include "FadeEffectScript.h"
#include "UIButton.h"
#include "../Engine/SceneManager.h"
#include "../Engine/SoundSystem.h"

DialogScene0::DialogScene0(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene0", Object::ObjectType::UI,
        1, 6, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene0.csv");
    dialog->CreateScript<D2DBitMapFontScript>();

    skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("DialogScene1");});

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

void DialogScene0::Enter()
{

}

void DialogScene0::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);  // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("DialogScene1");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}

void DialogScene0::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    fading->SetActive(true);
    skipbutton->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::Scene0, eSoundChannel::BGM);
}
