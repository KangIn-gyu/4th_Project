#include "pch.h"
#include "TutorialScene.h"
#include "D2DBaseObj.h"

#include "../Engine/SceneManager.h"

#include "ClickNextBimapScript.h"
#include "FadeEffectScript.h"
#include "LoadingScene.h"

#include "UIButton.h"
#include "../Engine/SoundSystem.h"

TutorialScene::TutorialScene(std::string_view _Name) : Scene(_Name)
{
    std::vector<std::string> bitmapFilePaths;
    std::string basePath = "TutorialScene/Textures/";
    for (int i = 1; i <= 14; ++i) // 1_Tutorial.png ~ 10_Tutorial.png
    {
        bitmapFilePaths.push_back(basePath + std::to_string(i) + "_Tutorial.png");
    }
    tutorialBitmap = CreatorObject<D2DBaseObj>("TutorialScene", Object::ObjectType::UI, bitmapFilePaths);
    tutorialBitmap->CreateScript<ClickNextBimapScript>();

    //SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);

    skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("LoadingScene");});

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>();

    tutorialBitmap->SetD2DLayerOrder(0);
    fading->SetD2DLayerOrder(5);
    skipbutton->SetD2DLayerOrder(1);

    tutorialBitmap->SetActive(false);
    fading->SetActive(false);
    skipbutton->SetActive(false);
}

void TutorialScene::Enter()
{

}

void TutorialScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    if (true == tutorialBitmap->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("LoadingScene");
        tutorialBitmap->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}
void TutorialScene::ResetInformation()
{
    Scene::ResetInformation();
    tutorialBitmap->SetActive(true);
    fading->SetActive(true);
    skipbutton->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
    static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("DialogScene2");

    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::Scene1, eSoundChannel::BGM);
}

void TutorialScene::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{

}
