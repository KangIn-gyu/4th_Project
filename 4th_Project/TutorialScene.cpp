#include "pch.h"
#include "TutorialScene.h"
#include "D2DBaseObj.h"

#include "../Engine/SceneManager.h"

#include "ClickNextBimapScript.h"
#include "FadeEffectScript.h"

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
    tutorialBitmap->SetActive(false);

    //SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);

    //static_cast<FadeEffectScript*>(test->script)->StartFadeOut();
    // 신아 / 세환 오면 버튼 물어보기
    skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),
        []() {SCENEMANAGER->ChangeScene("DialogScene2");});

    skipbutton->SetActive(false);

}
void TutorialScene::Enter()
{
}

void TutorialScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void TutorialScene::ResetInformation()
{
    skipbutton->SetActive(true);
    tutorialBitmap->SetActive(true);
}

void TutorialScene::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
    if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D1))
    {
        SCENEMANAGER->ChangeScene("DialogScene1");
    }
}
