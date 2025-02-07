#include "pch.h"
#include "TestScene.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"
#include "../Engine/SceneManager.h"
#include "Button.h"
#include "BlackJack.h"
#include "UIButton.h"
#include "TestObj.h"
#include "TestObj2.h"


TestScene::TestScene(std::string_view _Name) : Scene(_Name)
{
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	////ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//ObjectCreator<TestObj>("Deck", Object::ObjectType::Basic);
	//<TestObj>("Deck", Object::ObjectType::Basic);
}

void TestScene::Enter()
{
	//SceneLoader loader;
	//loader.Load("../4th_Project/2345.json");
	//loader.ImportUnityScene("../4th_Project/2345.json", this);
	//CreatorObject<TestObj2>("IngameMods", Object::ObjectType::Background);
	//CreatorObject<Dealer>("Dealer", Object::ObjectType::Background);
	
	Cards({ 0,0,0 });

	cardrot.Init(BLACKJACK->deck);
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(0, 200, 0), []() {}); //{SCENEMANAGER->ChangeScene("GAMBLE");});
	//CreatorObject<UIButton>("bu", Object::ObjectType::UI, "STAGE1/UI/mybutton2.png", []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	//CreatorObject<TestObj>("Font", Object::ObjectType::UI);

}

void TestScene::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);
	cardrot.Update(_deltaTime, BLACKJACK->deck->cards);
}

void TestScene::Cards(DXMath::Vector3 spos)
{
	//auto test = CreatorObject<Card>("Clover_Ace", Object::ObjectType::Basic);
	//
	float rotat = 90;
	float eulerAngle = DirectX::XMConvertToRadians(rotat);
	DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
	//auto testTrans = test->GetComponent<TransformComponent>();
	//testTrans->SetPosition(pos);
	//testTrans->SetQuaternion(eulerToQuaternion);
	//testTrans->SetScale({ 10.0f,10.0f,10.0f });

	BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);
	//BLACKJACK->deck->GetComponent<TransformComponent>()->SetPosition({ 100,0,0 });

	//CreatorObject<Card>()
}