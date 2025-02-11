#include "pch.h"
#include "TutorialScene.h"
#include "D2DBaseObj.h"

#include "../Engine/SceneManager.h"

#include "ClickNextBimapScript.h"
#include "FadeEffectScript.h"

#include "UIButton.h"
#include "../Engine/SoundSystem.h"
void TutorialScene::Enter()
{
    std::vector<std::string> bitmapFilePaths;
    std::string basePath = "TutorialScene/Textures/";
    for (int i = 1; i <= 14; ++i) // 1_Tutorial.png ~ 10_Tutorial.png
    {
        bitmapFilePaths.push_back(basePath + std::to_string(i) + "_Tutorial.png");
    }
	auto* tutorialBitmap = CreatorObject<D2DBaseObj>("TutorialScene", Object::ObjectType::UI, bitmapFilePaths);
    tutorialBitmap->CreateScript<ClickNextBimapScript>();
    //SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);

    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("DialogScene2");});

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>()->StartFadeOut();
    fading->SetD2DLayerOrder(5);
}

void TutorialScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void TutorialScene::ResetInformation()
{

}

void TutorialScene::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
    if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D1))
    {
        SCENEMANAGER->ChangeScene("DialogScene1");
    }
}
