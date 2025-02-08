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
#include "MyGameManager.h"
#include "GambleButton.h"
#include "SkillButton.h"
#include "ToopTip2D.h"
GambleScene::GambleScene(std::string_view _Name) : Scene(_Name)
{
	BLACKJACK->player = PLAYER;	
	CreatorObject<UIButton>("Bet", Object::ObjectType::UI, "UI/Button/Bet.png", DXMath::Vector2(500, 50), []() {BLACKJACK->Bet();});
	CreatorObject<GambleButton>("Open", Object::ObjectType::UI, DXMath::Vector2(800, 0), []() {ClickFunc::OpenButton();});
	CreatorObject<GambleButton>("Hit", Object::ObjectType::UI, DXMath::Vector2(1100, 0), []() {ClickFunc::HitButton();});
	CreatorObject<GambleButton>("Stay", Object::ObjectType::UI, DXMath::Vector2(1500, 0), []() {ClickFunc::StayButton();});

	CreatorObject<GambleButton>("Skill", Object::ObjectType::UI, DXMath::Vector2(1800, 0), []() {ClickFunc::OnSetSkillBtn();});

	CreatorObject<SkillButton>("Handfaster", Object::ObjectType::UI, DXMath::Vector2(500, 200), 3, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::fastEye);})->SetActive(false);
	CreatorObject<SkillButton>("Guts", Object::ObjectType::UI, DXMath::Vector2(800, 200), 1, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::guts);})->SetActive(false);
	CreatorObject<SkillButton>("Meditation", Object::ObjectType::UI, DXMath::Vector2(1100, 200), 2, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::meditation);})->SetActive(false);
	CreatorObject<SkillButton>("Insurance", Object::ObjectType::UI, DXMath::Vector2(1500, 200), 5, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::Insurance);})->SetActive(false);
}


void GambleScene::Enter()
{
	
	//GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetPosition({ -30.0f, 130.0f, -83.0f });
	//GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetQuaternion(DXMath::Quaternion::Quaternion(0.3f, 0.171f, -0.059f, 0.93f));

	BLACKJACK->dealer = CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic);
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 00, -13.0f, 50.0f });
	BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);
	BLACKJACK->deck->GetComponent<TransformComponent>()->SetPosition({ -60, 65, 0 });
	BLACKJACK->trashDeck = CreatorObject<Deck>("trashDeck", Object::ObjectType::Basic,false);

	BLACKJACK->Setstage(1);
	CreatorObject<TestObj>("Table", Object::ObjectType::Basic);

	auto test = CreatorObject<TestObj2>("Map", Object::ObjectType::Basic);
	auto deck = GetGameObject(Object::ObjectType::Basic, "Deck");

	

	//클릭시 스킬4개 버튼 출력할 버튼
	

	CreatorObject<ToopTip2D>("Handfaster_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);
	CreatorObject<ToopTip2D>("Guts_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);
	CreatorObject<ToopTip2D>("Meditation_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);
	CreatorObject<ToopTip2D>("Insurance_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);

}



void GambleScene::Update(const float _deltaTime)
{
	//MYGAMEMANAGER->Update(_deltaTime);
	__super::Update(_deltaTime);
	
	BLACKJACK->Update(_deltaTime);
	
}
