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
#include "UIToggleBtn.h"
#include "JustBox.h"
#include "hogamdo.h"
LobbyScene::LobbyScene(std::string_view _Name) : Scene(_Name)
{
	
}
void LobbyScene::Enter()
{
	AddGameObject(Object::ObjectType::Basic, BLACKJACK->dealer);
	AddGameObject(Object::ObjectType::Basic, BLACKJACK->player);

	CreatorObject<D3DBaseObj>("Map", Object::ObjectType::Background, "Common/FBX/Map_test_Lowpoly.fbx");

	{
		talk1 = CreatorObject<D2DBaseObj>("Talk1", Object::ObjectType::UI,
			112, 112, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Talk_1.csv");
		talk1->CreateScript<D2DBitMapFontScript>();
		talk1->SetActive(false);
		talk1->SetD2DLayerOrder(0);

		talk2 = CreatorObject<D2DBaseObj>("Talk2", Object::ObjectType::UI,
			112, 112, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Talk_2.csv");
		talk2->CreateScript<D2DBitMapFontScript>();
		talk2->SetActive(false);
		talk2->SetD2DLayerOrder(0);

		talk3 = CreatorObject<D2DBaseObj>("Talk3", Object::ObjectType::UI,
			112, 112, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Talk_3.csv");
		talk3->CreateScript<D2DBitMapFontScript>();
		talk3->SetActive(false);
		talk3->SetD2DLayerOrder(0);

		talk4 = CreatorObject<D2DBaseObj>("Talk4", Object::ObjectType::UI,
			112, 112, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Talk_4.csv");
		talk4->CreateScript<D2DBitMapFontScript>();
		talk4->SetActive(false);
		talk4->SetD2DLayerOrder(0);

		dialogs.push_back(talk1);
		dialogs.push_back(talk2);
		dialogs.push_back(talk3);
		dialogs.push_back(talk4);
	}
	 //토크버튼  TODO: BLACKJACK-> 몇번째 매치인지 가져오기.
	mth = BLACKJACK->curStage;

	talkButton = CreatorObject<UIToggleBtn>("TalkButton", Object::ObjectType::UI,
		DXMath::Vector2(1400, 700), [this]() { static_cast<D2DBitMapFontScript*>(dialogs[mth]->script)->Reset(); dialogs[mth]->SetActive(true); SCENEMANAGER->isTalking = true;});

	//	매치버튼 -> 로딩거치기 
	matchButton = CreatorObject<UIToggleBtn>("REMatchButton", Object::ObjectType::UI,
		DXMath::Vector2(1400, 500), []() {SCENEMANAGER->ChangeScene("LoadingScene");});

	CreatorObject<hogamdo>("hogamdo", Object::ObjectType::UI, DXMath::Vector2(1330, 350));
	fading = CreatorObject<D2DBaseObj>("Fade", Object::ObjectType::UI);
	fading->GetComponent<D2DRenderComponent>()->Load2DImage("UI/FadeImage.png");
	fading->CreateScript<FadeEffectScript>();

	talkButton->SetD2DLayerOrder(3);
	matchButton->SetD2DLayerOrder(4);
	fading->SetD2DLayerOrder(5);

	talkButton->SetActive(false);
	matchButton->SetActive(false);
	fading->SetActive(false);

	CreatorObject<JustBox>("Box1", Object::ObjectType::Basic, DXMath::Vector3(300, 0, 700), DXMath::Vector3(3000, 500, 100), DXMath::Vector3(0, 0, 0));
	CreatorObject<JustBox>("Box2", Object::ObjectType::Basic, DXMath::Vector3(300, 0, 1250), DXMath::Vector3(3000, 500, 100), DXMath::Vector3(0, 0, 0));
	CreatorObject<JustBox>("Box3", Object::ObjectType::Basic, DXMath::Vector3(-400, 0, 1250), DXMath::Vector3(100, 500, 2000), DXMath::Vector3(0, 0, 0));
	CreatorObject<JustBox>("Box4", Object::ObjectType::Basic, DXMath::Vector3(1300, 0, 1250), DXMath::Vector3(100, 500, 2000), DXMath::Vector3(0, 0, 0));
	
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


	BLACKJACK->dealer->SetActive(true);
	BLACKJACK->player->SetActive(true);
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 900, 8, 1250.5 });
	BLACKJACK->dealer->GetComponent<ModelComponent>()->SetAnimation(1); // 기본 애니메이션 추가
	GetGameObject(Object::ObjectType::UI, "TalkButton")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "REMatchButton")->SetActive(false);
	Object* camera = GetGameObject(Object::ObjectType::Camera, 0);

	camera->GetComponent<CameraCompoent>()->MovingFlag(true);
	TransformComponent* cameratrans = camera->GetComponent<TransformComponent>();
	cameratrans->SetPosition({ 800, 160, 1000 });
	cameratrans->SetQuaternion({ 0,0,0,1 });


	fading->SetActive(true);
	talk1->SetActive(false);
	talk2->SetActive(false);
	talk3->SetActive(false);
	talk4->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "hogamdo")->SetActive(false);
	static_cast<FadeEffectScript*>(fading->script)->StartFadeOut();
	static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("GambleScene");

}
