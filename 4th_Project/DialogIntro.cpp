#include "pch.h"
#include "DialogIntro.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // ½ºÅ©¸³Æ®
#include "FadeEffectScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
void DialogIntro::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
    dialog = CreatorObject<D2DBaseObj>("DialogIntro", Object::ObjectType::UI,
        7, 23, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Intro.csv");
    dialog->CreateScript<D2DBitMapFontScript>();

    // 스킵버튼
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("DialogScene0");});

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>()->StartFadeOut();
    fading->SetD2DLayerOrder(5);

    //SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}

void DialogIntro::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        std::cout << "페이드 인이 불값이 됨\n";
	    static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("DialogScene0");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}
