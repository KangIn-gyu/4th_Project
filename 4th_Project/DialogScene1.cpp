#include "pch.h"
#include "DialogScene1.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" 
#include "FadeEffectScript.h" 
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
#include "../Engine/D2DRenderComponent.h"

#include "UIButton.h"
#include "SelectionDialogScript.h"

DialogScene1::DialogScene1(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene1", Object::ObjectType::UI,
        7, 23, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene1.csv", "TutorialScene");
    dialog->SetD2DLayerOrder(0);
	dialog->CreateScript<D2DBitMapFontScript>();

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>()->StartFadeOut();
    fading->SetD2DLayerOrder(5);

	// TODO : 해당 씬에서 이 스크립트가 맞음 테스트를 위해서 잠시 주석해놓음

    UIButton* skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("TutorialScene");});
    skipbutton->SetD2DLayerOrder(1);
    //SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
    dialog->SetActive(true);
}

void DialogScene1::Enter()
{
}

void DialogScene1::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        std::cout << "페이드 인이 불값이 됨\n";
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("TutorialScene");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}

void DialogScene1::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    ///SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}
