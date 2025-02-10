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

#include "D2DBaseObj.h"
#include "SelectionDialogScript.h"
#include "../Engine/D2DRenderComponent.h"
#include "DialogButton.h"
#include "../Engine/Engine.h"
#include "SelectionImageScript.h"
#include "SelectionScript.h"

GambleScene::GambleScene(std::string_view _Name) : Scene(_Name)
{
	BLACKJACK->player = CreatorObject<Player>("Player", Object::ObjectType::Basic);
	CreatorObject<UIButton>("Bet", Object::ObjectType::UI, "UI/Button/Bet.png", DXMath::Vector2(500, 50), []() {BLACKJACK->Bet();});
	CreatorObject<GambleButton>("Open", Object::ObjectType::UI, DXMath::Vector2(800, 0), []() {ClickFunc::OpenButton();});
	CreatorObject<GambleButton>("Hit", Object::ObjectType::UI, DXMath::Vector2(1100, 0), []() {ClickFunc::HitButton();});
	CreatorObject<GambleButton>("Stay", Object::ObjectType::UI, DXMath::Vector2(1500, 0), []() {ClickFunc::StayButton();});

	CreatorObject<GambleButton>("Skill", Object::ObjectType::UI, DXMath::Vector2(1800, 0), []() {ClickFunc::OnSetSkillBtn();});

	CreatorObject<SkillButton>("Handfaster", Object::ObjectType::UI, DXMath::Vector2(500, 200), 3, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::fastEye);})->SetActive(false);
	CreatorObject<SkillButton>("Guts", Object::ObjectType::UI, DXMath::Vector2(800, 200), 1, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::guts);})->SetActive(false);
	CreatorObject<SkillButton>("Meditation", Object::ObjectType::UI, DXMath::Vector2(1100, 200), 2, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::meditation);})->SetActive(false);
	CreatorObject<SkillButton>("Insurance", Object::ObjectType::UI, DXMath::Vector2(1500, 200), 5, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::Insurance);})->SetActive(false);
	auto ui = CreatorObject<D2DBaseObj>("RoundBet", Object::ObjectType::UI, DXMath::Vector2{ 300,300 },"UI/RoundBet.png", "Font/DNFBitBitv2.ttf");
	ui->CreateScript<JustFont>()->SetMessage(BLACKJACK->player->Bet());



}


void GambleScene::Enter()
{
	// TODO: 다이얼로그로 넘어갈떄 지금이 첫번쨰 겜블인지 두번쨰인지 알아야함. 정보저장이던 넘기기던 플래그를 세워야함.
	// TODO: 그리고 플레이어의 행동력 0 체크로 선택지 버튼을 출력함.
	
	//GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetPosition({ -30.0f, 130.0f, -83.0f });
	//GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetQuaternion(DXMath::Quaternion::Quaternion(0.3f, 0.171f, -0.059f, 0.93f));

	BLACKJACK->dealer = CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic);
	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 00, -13.0f, 150.0f });
	BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);
	BLACKJACK->deck->GetComponent<TransformComponent>()->SetPosition({ -60, 65, 0 });
	BLACKJACK->trashDeck = CreatorObject<Deck>("trashDeck", Object::ObjectType::Basic,false);

	BLACKJACK->Setstage(1);
	CreatorObject<TestObj>("Table", Object::ObjectType::Basic);

	auto test = CreatorObject<TestObj2>("Map", Object::ObjectType::Basic);
	auto deck = GetGameObject(Object::ObjectType::Basic, "Deck");

	Object* camera = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::Camera, 0);
	TransformComponent* cameratrans = camera->GetComponent<TransformComponent>();
	cameratrans->SetPosition({ 0,160, -100 });
	cameratrans->SetQuaternion({0,0,0,1});

	//클릭시 스킬4개 버튼 출력할 버튼
	CreatorObject<ToopTip2D>("Handfaster_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);
	CreatorObject<ToopTip2D>("Guts_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);
	CreatorObject<ToopTip2D>("Meditation_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);
	CreatorObject<ToopTip2D>("Insurance_ToolTip", Object::ObjectType::UI, DXMath::Vector2(50, 200))->SetActive(false);

	// 다이얼로그 
	auto* skilldialog1_1 = CreatorObject<D2DBaseObj>("Skill1_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer1_1.csv");
	skilldialog1_1->CreateScript<SelectionScript>();
	auto* skilldialog1_2 = CreatorObject<D2DBaseObj>("Skill2_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer1_2.csv");
	skilldialog1_2->CreateScript<SelectionScript>();
	auto* skilldialog2_1 = CreatorObject<D2DBaseObj>("Skill3_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer2_1.csv");
	skilldialog2_1->CreateScript<SelectionScript>();
	auto* skilldialog2_2 = CreatorObject<D2DBaseObj>("Skill4_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer2_2.csv");
	skilldialog2_2->CreateScript<SelectionScript>();
	auto* skilldialog3_1 = CreatorObject<D2DBaseObj>("Skill4_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer3_1.csv");
	skilldialog3_1->CreateScript<SelectionScript>();
	auto* skilldialog3_2 = CreatorObject<D2DBaseObj>("Skill4_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer3_2.csv");
	skilldialog3_2->CreateScript<SelectionScript>();
	auto* skilldialog4_1 = CreatorObject<D2DBaseObj>("Skill4_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer4_1.csv");
	skilldialog4_1->CreateScript<SelectionScript>();
	auto* skilldialog4_2 = CreatorObject<D2DBaseObj>("Skill4_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer4_2.csv");
	skilldialog4_2->CreateScript<SelectionScript>();
	
	skilldialog1_1->SetActive(false);
	skilldialog1_2->SetActive(false);
	skilldialog2_1->SetActive(false);
	skilldialog2_2->SetActive(false);
	skilldialog3_1->SetActive(false);
	skilldialog3_2->SetActive(false);
	skilldialog4_1->SetActive(false);
	skilldialog4_2->SetActive(false);

	auto dialogbutton1 = CreatorObject<DialogButton>("Pattern1_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog1_1]() {skilldialog1_1->SetActive(true); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton2 = CreatorObject<DialogButton>("Pattern1_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog1_2]() { skilldialog1_2->SetActive(true); BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton3 = CreatorObject<DialogButton>("Pattern2_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog2_1]() {skilldialog2_1->SetActive(true); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton4 = CreatorObject<DialogButton>("Pattern2_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog2_2]() { skilldialog2_2->SetActive(true);  BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton5 = CreatorObject<DialogButton>("Pattern3_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog3_1]() {skilldialog3_1->SetActive(true); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton6 = CreatorObject<DialogButton>("Pattern3_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog3_2]() { skilldialog3_2->SetActive(true);  BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton7 = CreatorObject<DialogButton>("Pattern4_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog4_1]() {skilldialog4_1->SetActive(true); BLACKJACK->curTurn = Turn::player; });
	auto dialogbutton8 = CreatorObject<DialogButton>("Pattern4_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog4_2]() { skilldialog4_2->SetActive(true);  BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });

	auto q1 =  CreatorObject<D2DBaseObj>("Question1", Object::ObjectType::UI);
	q1->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question1.png");
	q1->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	q1->CreateScript<SelectionImageScript>()->SetButton(dialogbutton1, dialogbutton2);
	auto q2 = CreatorObject<D2DBaseObj>("Question1", Object::ObjectType::UI);
	q2->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question2.png");
	q2->CreateScript<SelectionImageScript>()->SetButton(dialogbutton3, dialogbutton4);
	q2->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	auto q3 = CreatorObject<D2DBaseObj>("Question1", Object::ObjectType::UI);
	q3->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question3.png");
	q3->CreateScript<SelectionImageScript>()->SetButton(dialogbutton5, dialogbutton6);
	q3->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	auto q4 = CreatorObject<D2DBaseObj>("Question1", Object::ObjectType::UI);
	q4->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question4.png");
	q4->CreateScript<SelectionImageScript>()->SetButton(dialogbutton7, dialogbutton8);
	q4->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	
	BLACKJACK->SetDialog(q1);
	BLACKJACK->SetDialog(q2);
	BLACKJACK->SetDialog(q3);
	BLACKJACK->SetDialog(q4);
	
}



void GambleScene::Update(const float _deltaTime)
{
	//MYGAMEMANAGER->Update(_deltaTime);
	__super::Update(_deltaTime);
	
	BLACKJACK->Update(_deltaTime);
	
}

void GambleScene::ResetInformation()
{
	Scene::ResetInformation();
	//GambleScene::Enter();
}

