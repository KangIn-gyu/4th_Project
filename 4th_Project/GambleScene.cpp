#include "pch.h"
#include "GambleScene.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/TransformComponent.h"
//#include "../Engine/SceneLoader.h"
#include "../Engine/SceneManager.h"
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"
#include "Button.h"
#include "BlackJack.h"
#include "ClickFunc.h"
#include "UIButton.h"

GambleScene::GambleScene(std::string_view _Name) : Scene(_Name)
{
	BLACKJACK->player = PLAYER;	
}


void GambleScene::Enter()
{
	
	GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetPosition({ -30.0f, 130.0f, -83.0f });
	GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetQuaternion(DXMath::Quaternion::Quaternion(0.3f, 0.171f, -0.059f, 0.93f));
	BLACKJACK->dealer = ObjectCreator<Dealer>("Dealer", Object::ObjectType::Basic); 
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 500, -13.0f, 50.0f });
	BLACKJACK->deck = ObjectCreator<Deck>("Deck", Object::ObjectType::Basic);
	BLACKJACK->deck->GetComponent<TransformComponent>()->SetPosition({ -60, 65, 0 });
	BLACKJACK->trashDeck = ObjectCreator<Deck>("Deck", Object::ObjectType::Basic,false);
	BLACKJACK->Setstage(1);
	ObjectCreator<TestObj>("Table", Object::ObjectType::Basic);

	//auto test = ObjectCreator<TestObj2>("Map", Object::ObjectType::Basic);
	//test->GetComponent<TransformComponent>()->SetPosition({ 0, -10, 0 });
	auto deck = GetGameObject(Object::ObjectType::Basic, "Deck");

	
	ObjectCreator<UIButton>("bu1", Object::ObjectType::UI, "STAGE1/UI/mybutton2.png", DXMath::Vector2(500,0), []() {BLACKJACK->Bet();});
	ObjectCreator<UIButton>("bu2", Object::ObjectType::UI, "STAGE1/UI/mybutton3.png", DXMath::Vector2(800, 0), []() {ClickFunc::OpenButton();});
	ObjectCreator<UIButton>("bu3", Object::ObjectType::UI, "STAGE1/UI/mybutton4.png", DXMath::Vector2(1100, 0), []() {ClickFunc::HitButton();});
	ObjectCreator<UIButton>("bu4", Object::ObjectType::UI, "STAGE1/UI/Stay.png", DXMath::Vector2(1500, 0), []() {ClickFunc::StayButton();});

	//클릭시 스킬4개 버튼 출력할 버튼
	ObjectCreator<UIButton>("bu5", Object::ObjectType::UI, "STAGE1/UI/mybutton6.png", DXMath::Vector2(1800, 0), []() {ClickFunc::OnSetSkillBtn();});

	ObjectCreator<UIButton>("SKill1", Object::ObjectType::UI, "STAGE1/UI/mybutton7.png", DXMath::Vector2(500, 200), []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::fastEye);})->SetActive(false);
	ObjectCreator<UIButton>("SKill2", Object::ObjectType::UI, "STAGE1/UI/OnGut.png", DXMath::Vector2(800, 200), []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::guts);})->SetActive(false);
	ObjectCreator<UIButton>("SKill3", Object::ObjectType::UI, "STAGE1/UI/mybutton9.png", DXMath::Vector2(1100, 200), []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::meditation);})->SetActive(false);
	ObjectCreator<UIButton>("SKill4", Object::ObjectType::UI, "STAGE1/UI/mybutton10.png", DXMath::Vector2(1500, 200), []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::Insurance);})->SetActive(false);
}



void GambleScene::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);

	BLACKJACK->Update(_deltaTime);
}
