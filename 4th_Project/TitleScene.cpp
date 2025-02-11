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

void TitleScene::Enter()
{
    { // 3D Obj
        auto* map = CreatorObject<D3DBaseObj>("TitleScene/FBX/Map_Lowpoly.fbx", Object::ObjectType::Background, "TitleScene/FBX/Map_Lowpoly.fbx");
        Evelyn = CreatorObject<D3DAniObj>("TitleScene/FBX/Evelyn.fbx", Object::ObjectType::Basic, "TitleScene/FBX/Table.fbx");
        Evelyn->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1270.5});
        TargetPosition = { 900, 107, 1272 };
        mainCamera = GetGameObject(Object::ObjectType::Camera, "MainCamera");
        static_cast<CameraObject*>(mainCamera)->TitleFlag(true);
        // 트랜스폼 
        cameraTransformComponent = mainCamera->GetComponent<TransformComponent>();
        cameraTransformComponent->SetPosition({ 902.0f, 140.0f, 1154 });

        auto* cameraComponent = mainCamera->GetComponent<CameraCompoent>();
        cameraComponent->LookAt(TargetPosition);

        startX = cameraTransformComponent->GetPosition().x;
    }

    { // 2D Obj 타이틀 UI 만들기
      auto* titleLogo = CreatorObject<D2DBaseObj>("titleLogo", Object::ObjectType::UI);
      titleLogo->SetD2DLayerOrder(1);
      D2DRenderComponent* LogoD2DRenderComponent = titleLogo->GetComponent<D2DRenderComponent>();
      LogoD2DRenderComponent->Load2DImage("TitleScene/UI/1_Logo_Og.png");
      LogoD2DRenderComponent->Set2DImagePos(200, 100);

      std::vector<std::string> bitmapFilePath = {{"TitleScene/UI/0_Button_Click To Start_Toggle.png"}};
  
      auto* titleClick = CreatorObject<D2DBaseObj>("titleClick", Object::ObjectType::UI, bitmapFilePath);
      D2DRenderComponent* titleClickD2DRenderComponent = titleClick->GetComponent<D2DRenderComponent>();
      titleClickD2DRenderComponent->Load2DImage("TitleScene/UI/0_Button_Click To Start_Toggle.png");
      titleClickD2DRenderComponent->Set2DImagePos(700, 900);

      auto* Backgraund = CreatorObject<D2DBaseObj>("titleBackgraund", Object::ObjectType::UI);
      Backgraund->sceneName = "DialogIntroScene";
      Backgraund->CreateScript<ClickChangeSceneScript>();
      D2DRenderComponent* BackgraundD2DRenderComponent = Backgraund->GetComponent<D2DRenderComponent>();
      BackgraundD2DRenderComponent->Load2DImage("TitleScene/UI/2_Backgraund_Fog.png");
      BackgraundD2DRenderComponent->SetAlpha(0.3f);
    }
}

void TitleScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);

    // cameraMoveTween이 null이 아니면 계속 업데이트
     if (movement)
     { 
        movement = false;
        new DOTween(cameraTransformComponent->GetPosition().x, EasingEffect::Linear, StepAnimation::StepLoopPingPong, 20.f, startX, 950.0f);
     }
}

void TitleScene::ResetInformation()
{
    Scene::ResetInformation();
    Evelyn->GetComponent<ModelComponent>()->SetAnimation(8);

    movement = true; // DOTween 처리

    static_cast<CameraObject*>(mainCamera)->MovingFlag(false);

    RENDERER->lightTarget = { 900 , 100 , 1272 };
    RENDERER->lightPos = { 0, 100,0 };
    RENDERER->lightDir = { 0, -1.0f , 0 };

    SOUNDSYSTEM->PlayMusic(eSoundList::Main_Theme, eSoundChannel::BGM);
}
