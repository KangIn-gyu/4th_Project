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
#include "Dealer.h"
#include "BlackJack.h"

#include "../Engine/SceneManager.h"
TitleScene::TitleScene(std::string_view _Name) : Scene(_Name)
{
    { // 3D Obj
        map = CreatorObject<D3DBaseObj>("Common/FBX/Map_Lowpoly.fbx", Object::ObjectType::Background, "Common/FBX/Map_test_Lowpoly.fbx");
        map->SetActive(false);

        BLACKJACK->dealer = CreatorObject<Dealer>("Evelyn", Object::ObjectType::Basic);
        BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1270.5 });
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
    BLACKJACK->player = CreatorObject<Player>("Player", Object::ObjectType::Basic);
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
    //RENDERER->ClearSpotLight();

    BLACKJACK->dealer->GetComponent<ModelComponent>()->SetAnimation(7);
    BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1270.5 });
    BLACKJACK->curStage = 0; //타이틀씬 가면 플레이어 클리어횟수 초기화
    map->SetActive(true);

    titleLogo->SetActive(true);
    titleClick->SetActive(true);
    Backgraund->SetActive(true);

    Object* camera = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::Camera, 0);
    camera->GetComponent<CameraCompoent>()->MovingFlag(true);

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

    SpotLightData test2;
    test2.position = DXMath::Vector3(900.0f, 300.0f, 1290.0f);
    test2.direction = DXMath::Vector3(0.0f, -1.0f, 0.5f);
    test2.color = DXMath::Vector3(1.0f, 1.0f, 1.0f);
    test2.range = 500.0f;
    test2.innerCone = cos(DX::XMConvertToRadians(45.0f));
    test2.outerCone = cos(DX::XMConvertToRadians(60.0f));
    test2.intensity = 500.0f;

    //RENDERER->AddSpotLight(test2);

    SpotLightData test3;
    test3.position = DXMath::Vector3(900.0f, 300.0f, 990.0f);
    test3.direction = DXMath::Vector3(0.0f, 0.0f, 1.0f);
    test3.color = DXMath::Vector3(1.0f, 1.0f, 1.0f);
    test3.range = 500.0f;
    test3.innerCone = cos(DX::XMConvertToRadians(89.0f));
    test3.outerCone = cos(DX::XMConvertToRadians(90.0f));
    test3.intensity = 500.0f;

    //RENDERER->AddSpotLight(test3);


    map->SetActive(true);
    BLACKJACK->dealer->SetActive(true);
    BLACKJACK->player->SetActive(true);
    titleLogo->SetActive(true);
    titleClick->SetActive(true);
    Backgraund->SetActive(true);
}
