#include "pch.h"
#include "CreditScene.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "FadeEffectScript.h"
#include "LoopImageChangeScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
CreditScene::CreditScene(std::string_view _Name) : Scene(_Name)
{
    std::vector<std::string> img =
    {
        "CreditScene/Textures/Credit_1.png",
        "CreditScene/Textures/Credit_2.png",
        "CreditScene/Textures/Credit_3.png",
        "CreditScene/Textures/Credit_4.png"
    };

	loopImg = CreatorObject<D2DBaseObj>("creditImg", Object::ObjectType::UI, img);
    loopImg->CreateScript<LoopImageChangeScript>();

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>();
    fading->SetD2DLayerOrder(5);
    fading->SetActive(false);
}

void CreditScene::Enter()
{
}

void CreditScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}


void CreditScene::ResetInformation()
{
    Scene::ResetInformation();
    fading->SetActive(true);
    loopImg->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();

    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::Credit, eSoundChannel::BGM);
}
