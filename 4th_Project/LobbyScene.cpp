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
#include "FadeEffectScript.h"
#include "LoadingScene.h"
#include "ToopTip2D.h"
#include "../Engine/CameraCompoent.h"
LobbyScene::LobbyScene(std::string_view _Name) : Scene(_Name)
{
	
}
void LobbyScene::Enter()
{
	AddGameObject(Object::ObjectType::Basic, BLACKJACK->dealer);
	BLACKJACK->player = CreatorObject<Player>("Player", Object::ObjectType::Basic);
	CreatorObject<D3DBaseObj>("Map", Object::ObjectType::Background, "Common/FBX/Map_test_Lowpoly.fbx");
	dealer = CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic);
	dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1250.5 });


	//	토크버튼 ->  // "UI/Button/Talk.png", "UI/Button/Talk_Click.png" , "UI/Button/Talk_Toggle.png"
	// TODO: BLACKJACK-> 몇번째 매치인지 가져오기.
	// if (BLACKJACK->몇번째)
	// {
	//		int mth = BLACKJACK->몇번째 + 1;
	// 		SCENEMANAGER->ChangeScene("TalkScene"+ mth );
	// }
	talkButton = CreatorObject<UIButton>("Talk", Object::ObjectType::UI, "UI/Button/Talk.png",
		DXMath::Vector2(1400, 700), []() {SCENEMANAGER->ChangeScene("TalkScene1");});

	//	매치버튼 -> 로딩거치기 // "UI/Button/REMatch.png", "UI/Button/REMatch_Click.png" , "UI/Button/REMatch_Toggle.png"
	matchButton = CreatorObject<UIButton>("REMatch", Object::ObjectType::UI, "UI/Button/REMatch.png",
		DXMath::Vector2(1400, 500), []() {SCENEMANAGER->ChangeScene("LoadingScene");});

	fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
	fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
	fading->CreateScript<FadeEffectScript>();

	talkButton->SetD2DLayerOrder(3);
	matchButton->SetD2DLayerOrder(4);
	fading->SetD2DLayerOrder(5);

	talkButton->SetActive(false);
	matchButton->SetActive(false);
	fading->SetActive(false);
	dealer->SetActive(false);
}

void LobbyScene::Update(const float _deltaTime)
{
    Scene::Update(_deltaTime);

}

void LobbyScene::ResetInformation()
{
	Scene::ResetInformation();

	
	//SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::Basic,"TitleScene/FBX/Evelyn.fbx")->GetComponent<ModelComponent>()->SetAnimation(8);
	SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
	SOUNDSYSTEM->PlayMusic(eSoundList::Lobby, eSoundChannel::BGM);
	// TOOD : 노래 넣어야 됨
	


	BLACKJACK->dealer->SetActive(true);
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1250.5 });
	BLACKJACK->dealer->GetComponent<ModelComponent>()->SetAnimation(1); // 기본 애니메이션 추가

	GetGameObject(Object::ObjectType::UI, "Handfaster_ToolTip")->SetActive(false); // TODO : 세환이가 버튼이라고 명시해달라고 함
	Object* camera = GetGameObject(Object::ObjectType::Camera, 0);

	camera->GetComponent<CameraCompoent>()->MovingFlag(true);
	TransformComponent* cameratrans = camera->GetComponent<TransformComponent>();
	cameratrans->SetPosition({ 0,160, -100 });
	cameratrans->SetQuaternion({ 0,0,0,1 });

	
	talkButton->SetActive(true);
	fading->SetActive(true);
	matchButton->SetActive(true);
	dealer->SetActive(true);
	static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
	static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("GambleScene");

	SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
}
