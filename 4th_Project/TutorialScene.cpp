#include "pch.h"
#include "TutorialScene.h"
#include "D2DBaseObj.h"

#include "../Engine/SceneManager.h"

#include "ClickNextBimapScript.h"
#include "FadeEffectScript.h"

#include "UIButton.h"

void TutorialScene::Enter()
{
//    std::vector<std::string> bitmapFilePaths;
//    std::string basePath = "TutorialScene/Textures/";
//    for (int i = 1; i <= 14; ++i) // 1_Tutorial.png ~ 10_Tutorial.png
//    {
//        bitmapFilePaths.push_back(basePath + std::to_string(i) + "_Tutorial.png");
//    }
//	auto* tutorialBitmap = CreatorObject<D2DBaseObj>("tutorial", Object::ObjectType::UI, bitmapFilePaths);
//    tutorialBitmap->CreateScript<ClickNextBimapScript>();

    std::vector<std::string> str = { "UI/FadeImage.png" };
    auto* test = CreatorObject<D2DBaseObj>("faf", Object::ObjectType::UI, str);
    test->CreateScript<FadeEffectScript>();
    static_cast<FadeEffectScript*>(test->script)->SetFadeSpeed(0.2f);
    static_cast<FadeEffectScript*>(test->script)->StartFadeIn();

    std::cout << "이때 씬체인지하기" << std::endl;

    static_cast<FadeEffectScript*>(test->script)->StartFadeOut();
    // 신아 / 세환 오면 버튼 물어보기
//    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,"TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(0, 0),[]() {SCENEMANAGER->ChangeScene("GAMBLE");});
}

void TutorialScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void TutorialScene::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
    if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D1))
    {
        SCENEMANAGER->ChangeScene("DialogScene1");
    }
}
