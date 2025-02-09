#include "pch.h"
#include "TitleScene.h"
#include "D3DBaseObj.h"
#include "D3DAniObj.h"
#include "D2DBaseObj.h"

#include "../Engine/DOTween.h"
#include "../Engine/CameraObject.h"
#include "../Engine/TransformComponent.h"
void TitleScene::Enter()
{
    { // 3D Obj
        auto* map = CreatorObject<D3DBaseObj>("TitleScene/FBX/Map_Lowpoly.fbx", Object::ObjectType::Background, "TitleScene/FBX/Map_Lowpoly.fbx");
        auto* Evelyn = CreatorObject<D3DAniObj>("TitleScene/FBX/Evelyn.fbx", Object::ObjectType::Basic, "TitleScene/FBX/Evelyn.fbx");
        Evelyn->GetComponent<TransformComponent>()->SetPosition({ 0,0, 508 });

        mainCamera = GetGameObject(Object::ObjectType::Camera, "MainCamera");
        cameraComponent = mainCamera->GetComponent<TransformComponent>();

        startZ = cameraComponent->GetPosition().z;
        cameraComponent->SetPosition({ -20.0f, 120.0f, startZ });
    }

    { // 2D Obj 타이틀 UI 만들기
        CreatorObject<D2DBaseObj>("titleUI", Object::ObjectType::UI, "");
    }
}

void TitleScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);

    // cameraMoveTween이 null이 아니면 계속 업데이트
    if (movement)
    { 
        movement = false;
        new DOTween(cameraComponent->GetPosition().z, EasingEffect::Linear, StepAnimation::StepOnceForward, 10.f, startZ, 300.0f);
    }

    std::cout << cameraComponent->GetPosition().z << "\n";
}

void TitleScene::ResetInformation()
{
    Scene::ResetInformation();
    movement = true;
}
