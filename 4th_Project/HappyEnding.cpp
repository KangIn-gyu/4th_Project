#include "pch.h"
#include "HappyEnding.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "FadeEffectScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
HappyEnding::HappyEnding(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("HappyEnding", Object::ObjectType::UI,
        22, 103, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/HappyEnding.csv");

    dialog->CreateScript<D2DBitMapFontScript>();
    skipbutton = CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("Epilogue");});

    // 페이드효과 밝아지기
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>();

    dialog->SetD2DLayerOrder(0);
    fading->SetD2DLayerOrder(5);
    skipbutton->SetD2DLayerOrder(1);

    dialog->SetActive(false);
    fading->SetActive(false);
    skipbutton->SetActive(false);
}

void HappyEnding::Enter()
{
}

void HappyEnding::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
    // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("Epilogue");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}


void HappyEnding::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    fading->SetActive(true);
    skipbutton->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
}
