#include "pch.h"
#include "CreditScene.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "FadeEffectScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
#include "../Engine/SoundSystem.h"
CreditScene::CreditScene(std::string_view _Name) : Scene(_Name)
{
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
    // 어두워지기
}


void CreditScene::ResetInformation()
{
    Scene::ResetInformation();
    fading->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();

    SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
    SOUNDSYSTEM->PlayMusic(eSoundList::Credit, eSoundChannel::BGM);
}
