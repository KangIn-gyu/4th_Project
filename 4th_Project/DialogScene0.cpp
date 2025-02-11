#include "pch.h"
#include "DialogScene0.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h"
#include "FadeEffectScript.h"
#include "UIButton.h"
#include "../Engine/SceneManager.h"

void DialogScene0::Enter()
{
	Scene::Enter();
    auto* dialog = CreatorObject<D2DBaseObj>("DialogScene0", Object::ObjectType::UI,
		1,6,"Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Scene0.csv");

    dialog->CreateScript<D2DBitMapFontScript>();
    CreatorObject<UIButton>("Skip", Object::ObjectType::UI,
        "TutorialScene/UI/UI 43_Skip.png", DXMath::Vector2(1730, 50), DXMath::Vector2(150, 45),
        []() {SCENEMANAGER->ChangeScene("DialogScene1");});

    // 페이드효과
    fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
    fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
    fading->CreateScript<FadeEffectScript>()->StartFadeOut();
    fading->SetD2DLayerOrder(5);

}

void DialogScene0::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);  // 어두워지기
    if (true == dialog->GetComponent<D2DRenderComponent>()->IsFadeIn)
    {
        std::cout << "페이드 인이 불값이 됨\n";
        static_cast<FadeEffectScript*>(fading->script)->StartFadeIn("DialogScene1");
        dialog->GetComponent<D2DRenderComponent>()->IsFadeIn = false;
    }
}
