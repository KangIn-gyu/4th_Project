#include "pch.h"
#include "TutorialScene.h"
#include "D2DBaseObj.h"
#include "ClickNextBimapScript.h"

#include "../Engine/SceneManager.h"
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
	auto* tutorialBitmap = CreatorObject<D2DBaseObj>("tutorial", Object::ObjectType::UI, bitmapFilePaths);
    tutorialBitmap->CreateScript<ClickNextBimapScript>();
    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);

    // 신아 / 세환 오면 버튼 물어보기
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,"TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(50, 50),[]() {SCENEMANAGER->ChangeScene("GAMBLE");});
}

void TutorialScene::Update(const float _deltaTime)
{

}

void TutorialScene::ResetInformation()
{
    SetState(true);
}

void TutorialScene::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
    if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D1))
    {
        SCENEMANAGER->ChangeScene("DialogScene1");
    }
}
