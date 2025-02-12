#include "pch.h"
#include "LobbyScene.h"
#include "../Engine/TransformComponent.h"
#include "UIButton.h"
#include "D2DBaseObj.h"
#include "D2DBitMapFontScript.h"
#include "D3DBaseObj.h"
#include "../Engine/SceneManager.h"
#include "../Engine/SoundSystem.h"
#include "D3DAniObj.h"
#include "../Engine/ModelComponent.h"
#include "Dealer.h"
#include "Player.h"
#include "BlackJack.h"
#include "ToopTip2D.h"

LobbyScene::LobbyScene(std::string_view _Name) : Scene(_Name)
{
	
}
void LobbyScene::Enter()
{
	AddGameObject(Object::ObjectType::Basic, BLACKJACK->dealer);
	BLACKJACK->player = CreatorObject<Player>("Player", Object::ObjectType::Basic);
	CreatorObject<D3DBaseObj>("Map", Object::ObjectType::Background, "Common/FBX/Map_test_Lowpoly.fbx");
	
	CreatorObject<ToopTip2D>("Handfaster_ToolTip", Object::ObjectType::UI, DXMath::Vector2(1400, 500))->SetActive(false);
}

void LobbyScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void LobbyScene::ResetInformation()
{

	//
	//SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::Basic,"TitleScene/FBX/Evelyn.fbx")->GetComponent<ModelComponent>()->SetAnimation(8);
	SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
	// TOOD : 노래 넣어야 됨

	BLACKJACK->dealer->SetActive(true);
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1250.5 });
	BLACKJACK->dealer->GetComponent<ModelComponent>()->SetAnimation(1); // 기본 애니메이션 추가

	GetGameObject(Object::ObjectType::UI, "Handfaster_ToolTip")->SetActive(false); // TODO : 세환이가 버튼이라고 명시해달라고 함
	Object* camera = GetGameObject(Object::ObjectType::Camera, 0);
	TransformComponent* cameratrans = camera->GetComponent<TransformComponent>();
	cameratrans->SetPosition({ 0,160, -100 });
	cameratrans->SetQuaternion({ 0,0,0,1 });

	//BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1250.5 });
	//GetGameObject(Object::ObjectType::Basic, "Dealer")->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1250.5 });
}
