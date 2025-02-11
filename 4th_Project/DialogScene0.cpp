#include "pch.h"
#include "DialogScene0.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h"
#include "FadeEffectScript.h"
#include "UIButton.h"
#include "../Engine/SceneManager.h"

DialogScene0::DialogScene0(std::string_view _Name) : Scene(_Name)
{
    dialog = CreatorObject<D2DBaseObj>("DialogScene0", Object::ObjectType::UI,
        1, 6, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene0.csv", "DialogScene1");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("DialogScene1");});

    dialog->SetActive(false);

    // í˜ì´ë“œíš¨ê³¼
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->SetD2DLayerOrder(5);
    fading->SetActive(false);
}

void DialogScene0::Enter()
{

}

void DialogScene0::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);  // ¾îµÎ¿öÁö±â
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        std::cout << "ÆäÀÌµå ÀÎÀÌ ºÒ°ªÀÌ µÊ\n";
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("DialogScene1");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}

void DialogScene0::ResetInformation()
{
    dialog->SetActive(true);
    fading->SetActive(true);
    static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
}
