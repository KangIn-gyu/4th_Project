#include "pch.h"
#include "CreditScene.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "FadeEffectScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
CreditScene::CreditScene(std::string_view _Name) : Scene(_Name)
{

    dialog = CreatorObject<D2DBaseObj>("CreditScene", Object::ObjectType::UI,
        1, 111, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/CreditScene.csv");
    dialog->CreateScript<D2DBitMapFontScript>();


    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>();

    dialog->SetD2DLayerOrder(0);
    fading->SetD2DLayerOrder(5);

    dialog->SetActive(false);
    fading->SetActive(false);
}

void CreditScene::Enter()
{
}

void CreditScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("LobbyScene");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}


void CreditScene::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    fading->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
}
