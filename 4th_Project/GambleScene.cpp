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
#include "JustFont.h"
#include "D2DBaseObj.h"
#include "SelectionDialogScript.h"
#include "../Engine/D2DRenderComponent.h"
#include "DialogButton.h"
#include "LoadingScene.h"
#include "../Engine/Engine.h"
#include "SelectionImageScript.h"
#include "SelectionScript.h"
#include "../Engine/CameraCompoent.h"

#include "../Engine/ModelComponent.h"
GambleScene::GambleScene(std::string_view _Name) : Scene(_Name)
{
}



void GambleScene::Enter()
{
	// TODO: 다이얼로그로 넘어갈떄 지금이 첫번쨰 겜블인지 두번쨰인지 알아야함. 정보저장이던 넘기기던 플래그를 세워야함.
	// TODO: 그리고 플레이어의 행동력 0 체크로 선택지 버튼을 출력함.

	BLACKJACK->player = CreatorObject<Player>("Player", Object::ObjectType::Basic);
	BLACKJACK->dealer = CreatorObject<Dealer>("Dealer", Object::ObjectType::Basic);
	//GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetPosition({ -30.0f, 130.0f, -83.0f });
	//GetGameObject(Object::ObjectType::Camera)->GetComponent<TransformComponent>()->SetQuaternion(DXMath::Quaternion::Quaternion(0.3f, 0.171f, -0.059f, 0.93f));

	BLACKJACK->dealer->GetComponent<TransformComponent>()->SetPosition({ 00, -13.0f, -400.0f });

	BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);
	BLACKJACK->deck->GetComponent<TransformComponent>()->SetPosition({ -60, 105, -500 });
	BLACKJACK->trashDeck = CreatorObject<Deck>("trashDeck", Object::ObjectType::Basic, false);

	BLACKJACK->Setstage(1);
	CreatorObject<TestObj>("Table", Object::ObjectType::Basic);

	auto test = CreatorObject<TestObj2>("Map", Object::ObjectType::Background);
	auto deck = GetGameObject(Object::ObjectType::Basic, "Deck");



	//클릭시 스킬4개 버튼 출력할 버튼
	CreatorObject<ToopTip2D>("Handfaster_ToolTip", Object::ObjectType::UI, DXMath::Vector2(1460, 370))->SetActive(false);
	CreatorObject<ToopTip2D>("Guts_ToolTip", Object::ObjectType::UI, DXMath::Vector2(1430, 430))->SetActive(false);
	CreatorObject<ToopTip2D>("Meditation_ToolTip", Object::ObjectType::UI, DXMath::Vector2(1430, 490))->SetActive(false);
	CreatorObject<ToopTip2D>("Insurance_ToolTip", Object::ObjectType::UI, DXMath::Vector2(1460, 550))->SetActive(false);

	CreatorObject<UIButton>("Bet", Object::ObjectType::UI, "UI/Button/Bet.png", DXMath::Vector2(500, 50), []() {BLACKJACK->Bet(); });
	CreatorObject<GambleButton>("Open", Object::ObjectType::UI, DXMath::Vector2(1650, 360), []() {ClickFunc::OpenButton(); });
	CreatorObject<GambleButton>("Hit", Object::ObjectType::UI, DXMath::Vector2(1745, 440), []() {ClickFunc::HitButton(); });
	CreatorObject<GambleButton>("Skill", Object::ObjectType::UI, DXMath::Vector2(1650, 520), []() {ClickFunc::OnSetSkillBtn(); });
	CreatorObject<GambleButton>("Stay", Object::ObjectType::UI, DXMath::Vector2(1650, 680), []() {ClickFunc::StayButton(); });
	CreatorObject<GambleButton>("DoubleDown", Object::ObjectType::UI, DXMath::Vector2(1745, 600), []() {});

	CreatorObject<SkillButton>("Handfaster", Object::ObjectType::UI, DXMath::Vector2(1460, 470), 3, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::fastEye); })->SetActive(false);
	CreatorObject<SkillButton>("Guts", Object::ObjectType::UI, DXMath::Vector2(1430, 530), 1, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::guts); })->SetActive(false);
	CreatorObject<SkillButton>("Meditation", Object::ObjectType::UI, DXMath::Vector2(1430, 590), 2, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::meditation); })->SetActive(false);
	CreatorObject<SkillButton>("Insurance", Object::ObjectType::UI, DXMath::Vector2(1460, 650), 5, []() {ClickFunc::SetPlayerSkill(PLAYER, PSkill::Insurance); })->SetActive(false);
	auto ui = CreatorObject<D2DBaseObj>("RoundBet", Object::ObjectType::UI, DXMath::Vector2{ 885, 100 }, "UI/RoundBet.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui->CreateScript<JustFont>()->SetMessage(&BLACKJACK->betMoney);

	auto ui2 = CreatorObject<D2DBaseObj>("multiple", Object::ObjectType::UI, DXMath::Vector2{ 910, 20 }, "UI/Button/invisible.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui2->CreateScript<JustFont>()->SetMessage(&BLACKJACK->magnification);
	static_cast<JustFont*>(ui2->script)->SetString(L"X");

	auto ui3 = CreatorObject<D2DBaseObj>("Skill_Energe", Object::ObjectType::UI, DXMath::Vector2{ 1650, 220 }, "UI/Skill_Energe.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui3->CreateScript<JustFont>()->SetMessage(&BLACKJACK->player->skillPoint);

	CreatorObject<UIButton>("ALLIN", Object::ObjectType::UI, "UI/Button/ALL_IN.png", DXMath::Vector2(1600, 850), []() {});

	auto ui4 = CreatorObject<D2DBaseObj>("PlayerChipBox", Object::ObjectType::UI, DXMath::Vector2{ 1120, 120 }, "UI/ChipBox.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui4->CreateScript<JustFont>()->SetMessage(&BLACKJACK->player->chip);
	auto ui5 = CreatorObject<D2DBaseObj>("DealerChipBox", Object::ObjectType::UI, DXMath::Vector2{ 320, 120 }, "UI/ChipBox.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui5->CreateScript<JustFont>()->SetMessage(&BLACKJACK->dealer->chip);
	// 딜러 칩 어디서 설정하는지 그거 순서 문제
	auto ui6 = CreatorObject<D2DBaseObj>("PlayerNum", Object::ObjectType::UI, DXMath::Vector2{ 380, 850 }, "UI/Num.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui6->CreateScript<JustFont>()->SetMessage(&BLACKJACK->player->score);

	auto ui7 = CreatorObject<D2DBaseObj>("RoundBet", Object::ObjectType::UI, DXMath::Vector2{ 1600, 1000 }, "UI/RaiseBar.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui7->CreateScript<JustFont>()->SetMessage(BLACKJACK->player->Bet());

	CreatorObject<UIButton>("DealerChip", Object::ObjectType::UI, "UI/Chip.png", DXMath::Vector2{ 1580, 1000 }, []() {});

	auto ui8 = CreatorObject<D2DBaseObj>("DealerNum", Object::ObjectType::UI, DXMath::Vector2{ 680, 620 }, "UI/Num.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui8->CreateScript<JustFont>()->SetMessage(&BLACKJACK->dealer->score);

	auto* DealerWin = CreatorObject<UIButton>("DealerWin", Object::ObjectType::UI, "UI/Lose.png", DXMath::Vector2{ 0,200 }, []() {});

	auto* PlayerWin = CreatorObject<UIButton>("PlayerWin", Object::ObjectType::UI, "UI/Win.png", DXMath::Vector2{ 0,200 }, []() {});

	auto ui9 = CreatorObject<D2DBaseObj>("BetResult", Object::ObjectType::UI, DXMath::Vector2{ 1500, 500 }, "UI/ResultBox.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui9->CreateScript<JustFont>()->SetMessage(&BLACKJACK->betMoney);
	static_cast<JustFont*>(ui9->script)->SetTextSize(50.0f);

	auto ui10 = CreatorObject<D2DBaseObj>("BetMag", Object::ObjectType::UI, DXMath::Vector2{ 1500, 735 }, "UI/ResultBox.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui10->CreateScript<JustFont>()->SetMessage(&BLACKJACK->magnification);
	static_cast<JustFont*>(ui10->script)->SetTextSize(100.0f);

	auto ui11 = CreatorObject<D2DBaseObj>("Result", Object::ObjectType::UI, DXMath::Vector2{ 1500, 885 }, "UI/ResultBox.png", "Font/GyeonggiMillenniumBackground_Regular.ttf");
	ui11->CreateScript<JustFont>()->SetMessage(&BLACKJACK->sum);
	static_cast<JustFont*>(ui11->script)->SetTextSize(100.0f);

	DealerWin->SetOnClick([DealerWin, ui9, ui10, ui11]()
		{ BLACKJACK->isRoundOver = true; DealerWin->SetActive(false);
	ui9->SetActive(false); ui10->SetActive(false); ui11->SetActive(false); });

	PlayerWin->SetOnClick([PlayerWin, ui9, ui10, ui11]()
		{ BLACKJACK->isRoundOver = true; PlayerWin->SetActive(false);
	ui9->SetActive(false); ui10->SetActive(false); ui11->SetActive(false); });


	auto* ButtonTen = CreatorObject<UIButton>("ButtonTen", Object::ObjectType::UI, "UI/Button10.png", DXMath::Vector2{ 1000,900 }, []() {});
	auto* ButtonOne = CreatorObject<UIButton>("ButtonOne", Object::ObjectType::UI, "UI/Button1.png", DXMath::Vector2{ 920,900 }, []() {});

	// ui 테스트용
	CreatorObject<UIButton>("PlayerFace", Object::ObjectType::UI, "UI/PlayerFace.png", DXMath::Vector2{ 1620, 50 }, []() {});
	CreatorObject<UIButton>("DealerFace", Object::ObjectType::UI, "UI/DealerFace.png", DXMath::Vector2{ 80, 50 }, []() {});
	CreatorObject<UIButton>("PlayerChip", Object::ObjectType::UI, "UI/Chip.png", DXMath::Vector2{ 1500, 50 }, []() {});
	CreatorObject<UIButton>("DealerChip", Object::ObjectType::UI, "UI/Chip.png", DXMath::Vector2{ 380, 50 }, []() {});

	// 다이얼로그 
	auto* skilldialog1_1 = CreatorObject<D2DBaseObj>("Skill1_1_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer1_1.csv");
	skilldialog1_1->CreateScript<SelectionScript>();
	auto* skilldialog1_2 = CreatorObject<D2DBaseObj>("Skill1_2_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer1_2.csv");
	skilldialog1_2->CreateScript<SelectionScript>();
	auto* skilldialog2_1 = CreatorObject<D2DBaseObj>("Skill2_1_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer2_1.csv");
	skilldialog2_1->CreateScript<SelectionScript>();
	auto* skilldialog2_2 = CreatorObject<D2DBaseObj>("Skill2_2_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer2_2.csv");
	skilldialog2_2->CreateScript<SelectionScript>();
	auto* skilldialog3_1 = CreatorObject<D2DBaseObj>("Skill3_1_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer3_1.csv");
	skilldialog3_1->CreateScript<SelectionScript>();
	auto* skilldialog3_2 = CreatorObject<D2DBaseObj>("Skill3_2Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer3_2.csv");
	skilldialog3_2->CreateScript<SelectionScript>();
	auto* skilldialog4_1 = CreatorObject<D2DBaseObj>("Skill4_1_Dialog", Object::ObjectType::UI, 112, 113, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer4_1.csv");
	skilldialog4_1->CreateScript<SelectionScript>();
	auto* skilldialog4_2 = CreatorObject<D2DBaseObj>("Skill4_2_Dialog", Object::ObjectType::UI, 113, 114, "Font/GyeonggiMillenniumBackground_Regular.ttf", "DialogScenes/CSV/Dealer4_2.csv");
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
		[skilldialog1_1]() {skilldialog1_1->SetActive(true); BLACKJACK->curTurn = Turn::player; BLACKJACK->dealer->SetSkill(); });
	auto dialogbutton2 = CreatorObject<DialogButton>("Pattern1_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog1_2]() { skilldialog1_2->SetActive(true); BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });

	auto dialogbutton3 = CreatorObject<DialogButton>("Pattern2_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog2_1]() {skilldialog2_1->SetActive(true); BLACKJACK->curTurn = Turn::player; BLACKJACK->dealer->SetSkill(); });
	auto dialogbutton4 = CreatorObject<DialogButton>("Pattern2_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog2_2]() { skilldialog2_2->SetActive(true);  BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });

	auto dialogbutton5 = CreatorObject<DialogButton>("Pattern3_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog3_1]() {skilldialog3_1->SetActive(true); BLACKJACK->curTurn = Turn::player; BLACKJACK->dealer->SetSkill(); });
	auto dialogbutton6 = CreatorObject<DialogButton>("Pattern3_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog3_2]() { skilldialog3_2->SetActive(true);  BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });

	auto dialogbutton7 = CreatorObject<DialogButton>("Pattern4_1", Object::ObjectType::UI, DXMath::Vector2(100, 450),
		[skilldialog4_1]() {skilldialog4_1->SetActive(true); BLACKJACK->curTurn = Turn::player; BLACKJACK->dealer->SetSkill(); });
	auto dialogbutton8 = CreatorObject<DialogButton>("Pattern4_2", Object::ObjectType::UI, DXMath::Vector2(1200, 450),
		[skilldialog4_2]() { skilldialog4_2->SetActive(true);  BLACKJACK->dealer->Act(); BLACKJACK->curTurn = Turn::player; });

	auto q1 = CreatorObject<D2DBaseObj>("Question1", Object::ObjectType::UI);
	q1 = CreatorObject<D2DBaseObj>("Question1", Object::ObjectType::UI);
	q1->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question1.png");
	q1->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	q1->CreateScript<SelectionImageScript>()->SetButton(dialogbutton1, dialogbutton2);
	q1->SetActive(false);

	q2 = CreatorObject<D2DBaseObj>("Question2", Object::ObjectType::UI);
	q2->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question2.png");
	q2->CreateScript<SelectionImageScript>()->SetButton(dialogbutton3, dialogbutton4);
	q2->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	q2->SetActive(false);

	q3 = CreatorObject<D2DBaseObj>("Question3", Object::ObjectType::UI);
	q3->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question3.png");
	q3->CreateScript<SelectionImageScript>()->SetButton(dialogbutton5, dialogbutton6);
	q3->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);
	q3->SetActive(false);

	q4 = CreatorObject<D2DBaseObj>("Question4", Object::ObjectType::UI);
	q4->GetComponent<D2DRenderComponent>()->Load2DImage("UI/Question/Question4.png");
	q4->CreateScript<SelectionImageScript>()->SetButton(dialogbutton7, dialogbutton8);
	q4->GetComponent<D2DRenderComponent>()->Set2DImagePos(0, 580);

	q4->SetActive(false);

	BLACKJACK->SetDialog(q1);
	BLACKJACK->SetDialog(q2);
	BLACKJACK->SetDialog(q3);
	BLACKJACK->SetDialog(q4);
}



void GambleScene::Update(const float _deltaTime)
{
	//MYGAMEMANAGER->Update(_deltaTime);
	__super::Update(_deltaTime);

	BLACKJACK->Update(_deltaTime * 2.0);
}

void GambleScene::ResetInformation()
{
	Scene::ResetInformation();
	GetGameObject(Object::ObjectType::UI, "Meditation")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Insurance")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "DealerWin")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "PlayerWin")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Handfaster_ToolTip")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Guts_ToolTip")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Meditation_ToolTip")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Insurance_ToolTip")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill1_1_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill1_2_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill2_1_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill2_2_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill3_1_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill3_2Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill4_1_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Skill4_2_Dialog")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "BetResult")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "BetMag")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "Result")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "ButtonTen")->SetActive(false);
	GetGameObject(Object::ObjectType::UI, "ButtonOne")->SetActive(false);

	// 왜 여기 선언 해야하는지 진짜모름
	Object* camera = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::Camera, 0);
	camera->GetComponent<CameraCompoent>()->MovingFlag(true);
	TransformComponent* cameratrans = camera->GetComponent<TransformComponent>();
	float angle = DirectX::XMConvertToRadians(10.0f);
	DXMath::Quaternion quat = DXMath::Quaternion::CreateFromYawPitchRoll(0.0f, angle, 0.0f);
	cameratrans->SetQuaternion(quat);
	cameratrans->SetPosition({ 0, 140, -580 });

	GetGameObject(Object::ObjectType::Basic, "Deck")->GetComponent<TransformComponent>()->SetPosition({ -60, 105, -500 });
	//GambleScene::Enter();
	//dealer->GetComponent<ModelComponent>()->SetAnimation(1);


	static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("TitleScene");
}

