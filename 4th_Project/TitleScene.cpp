#include "pch.h"
#include "TitleScene.h"
#include "D3DBaseObj.h"
#include "D3DAniObj.h"
#include "D2DBaseObj.h"

#include "../Engine/DOTween.h"
#include "../Engine/CameraObject.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/CameraCompoent.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/Renderer.h"

#include "LoopImageChangeScript.h"
#include "../Engine/SoundSystem.h"
#include "ClickChangeSceneScript.h"

TitleScene::TitleScene(std::string_view _Name) : Scene(_Name)
{
    { // 3D Obj
        map = CreatorObject<D3DBaseObj>("Common/FBX/Map_Lowpoly.fbx", Object::ObjectType::Background, "Common/FBX/Map_test_Lowpoly.fbx");
        map->SetActive(false);

        Evelyn = CreatorObject<D3DAniObj>("Common/FBX/Evelyn.fbx", Object::ObjectType::Basic, "Common/FBX/Evelyn.fbx");
        Evelyn->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1270.5 });
 
        TargetPosition = { 900, 107, 1272 };
        mainCamera = GetGameObject(Object::ObjectType::Camera, "MainCamera");
        static_cast<CameraObject*>(mainCamera)->TitleFlag(true);

        cameraTransformComponent = mainCamera->GetComponent<TransformComponent>();
        cameraTransformComponent->SetPosition({ 902.0f, 140.0f, 1154 });

        auto* cameraComponent = mainCamera->GetComponent<CameraCompoent>();
        cameraComponent->LookAt(TargetPosition);

        startX = cameraTransformComponent->GetPosition().x;
    }

    { // 2D Obj
        titleLogo = CreatorObject<D2DBaseObj>("titleLogo", Object::ObjectType::UI);
        titleLogo->SetD2DLayerOrder(1);
        D2DRenderComponent* LogoD2DRenderComponent = titleLogo->GetComponent<D2DRenderComponent>();
        LogoD2DRenderComponent->Load2DImage("TitleScene/UI/1_Logo_Og.png");
        LogoD2DRenderComponent->Set2DImagePos(200, 100);
        titleLogo->SetActive(false);

        std::vector<std::string> bitmapFilePath = { {"TitleScene/UI/0_Button_Click To Start_Toggle.png"} };

        titleClick = CreatorObject<D2DBaseObj>("titleClick", Object::ObjectType::UI, bitmapFilePath);
        D2DRenderComponent* titleClickD2DRenderComponent = titleClick->GetComponent<D2DRenderComponent>();
        titleClickD2DRenderComponent->Load2DImage("TitleScene/UI/0_Button_Click To Start_Toggle.png");
        titleClickD2DRenderComponent->Set2DImagePos(700, 900);
        titleClick->SetActive(false);

        Backgraund = CreatorObject<D2DBaseObj>("titleBackgraund", Object::ObjectType::UI);
        Backgraund->sceneName = "DialogIntroScene";
        Backgraund->CreateScript<ClickChangeSceneScript>();
        D2DRenderComponent* BackgraundD2DRenderComponent = Backgraund->GetComponent<D2DRenderComponent>();
        BackgraundD2DRenderComponent->Load2DImage("TitleScene/UI/2_Backgraund_Fog.png");
        BackgraundD2DRenderComponent->SetAlpha(0.3f);
        Backgraund->SetActive(false);
    }
}

void TitleScene::Enter()
{

}

void TitleScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);

     if (movement)
     { 
        movement = false;
        new DOTween(cameraTransformComponent->GetPosition().x, EasingEffect::Linear, StepAnimation::StepLoopPingPong, 20.f, startX, 950.0f);
     }
}

void TitleScene::ResetInformation()
{
    Scene::ResetInformation();
    map->SetActive(true);
    Evelyn->SetActive(true);

    titleLogo->SetActive(true);
    titleClick->SetActive(true);
    Backgraund->SetActive(true);

    Evelyn->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1270.5 });
    Evelyn->GetComponent<ModelComponent>()->SetAnimation(7);

    cameraTransformComponent = mainCamera->GetComponent<TransformComponent>();
    cameraTransformComponent->SetPosition({ 902.0f, 140.0f, 1154 });
    cameraTransformComponent->SetQuaternion({ 0.0f, 0.0f, 0.0f, 1.0f });

    auto* cameraComponent = mainCamera->GetComponent<CameraCompoent>();
    cameraComponent->LookAt(TargetPosition);

    movement = true; 

    static_cast<CameraObject*>(mainCamera)->MovingFlag(false);

    RENDERER->lightTarget = { 900 , 100 , 1272 };
    RENDERER->lightPos = { 0, 100,0 };
    RENDERER->lightDir = { 0, -1.0f , 0 };
    RENDERER->upColor = true;
    SOUNDSYSTEM->PlayMusic(eSoundList::TitleScene, eSoundChannel::BGM);
}
