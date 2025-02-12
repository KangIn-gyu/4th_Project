#include "pch.h"
#include "EndingChoice.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h" // 스크립트
#include "DialogButton.h"
#include "FadeEffectScript.h"
#include "../Engine/SceneManager.h"
#include "UIButton.h"
EndingChoice::EndingChoice(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("EndingChoice", Object::ObjectType::UI,
        0,0, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/EndingChoice.csv");
    dialog->CreateScript<D2DBitMapFontScript>();

    happyButton = CreatorObject<DialogButton>("HappyEnding", Object::ObjectType::UI, DXMath::Vector2(1200, 450), []() {SCENEMANAGER->ChangeScene("HappyEnding");});
    badButton = CreatorObject<DialogButton>("BadEnding", Object::ObjectType::UI, DXMath::Vector2(100, 450), []() {SCENEMANAGER->ChangeScene("BadEnding");});

    // 페이드효과 밝아지기 여기 초이스씬 에서는 안함.
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>();

    dialog->SetD2DLayerOrder(0);
    happyButton->SetD2DLayerOrder(4);
    badButton->SetD2DLayerOrder(3);
    fading->SetD2DLayerOrder(5);

    dialog->SetActive(false);
    happyButton->SetActive(false);
    badButton->SetActive(false);
    fading->SetActive(false);
}

void EndingChoice::Enter()
{
}

void EndingChoice::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void EndingChoice::ResetInformation()
{
    Scene::ResetInformation();
    dialog->SetActive(true);
    fading->SetActive(true);
    happyButton->SetActive(true);
    badButton->SetActive(true);
}