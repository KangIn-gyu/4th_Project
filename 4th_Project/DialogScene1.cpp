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

void DialogScene1::Enter()
{
    // Font/DialogScene.ttf  // GyeonggiMillenniumBackground_Regular
	auto* dialog = CreatorObject<D2DBaseObj>("DialogScene1", Object::ObjectType::UI,
        7, 23,"Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene1.csv", "TutorialScene");
    dialog->SetD2DLayerOrder(0);


    ///
    auto fading = dialog->CreateScript<FadeEffectScript>();
    fading->StartFadeOut();
    ///


    dialog->CreateScript<D2DBitMapFontScript>();  // TODO : 해당 씬에서 이 스크립트가 맞음 테스트를 위해서 잠시 주석해놓음

    UIButton* skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("TutorialScene");});
    skipbutton->SetD2DLayerOrder(1);
    //SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}

void DialogScene1::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void DialogScene1::ResetInformation()
{
    Scene::ResetInformation();
    ///SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}
