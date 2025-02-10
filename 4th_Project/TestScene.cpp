#include "pch.h"
#include "../Engine/DirectXInput.h"
#include "TestScene.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneLoader.h"
#include "../Engine/SceneManager.h"
#include "Button.h"
#include "BlackJack.h"
#include "UIButton.h"
#include "TestObj.h"
#include "TestObj2.h"
#include "../Engine/CameraObject.h"

void TestScene::Enter()
{
	//SceneLoader loader;
	//loader.Load("../4th_Project/2345.json");
	//loader.ImportUnityScene("../4th_Project/2345.json", this);
	//CreatorObject<TestObj2>("IngameMods", Object::ObjectType::Background);
	//CreatorObject<Dealer>("Dealer", Object::ObjectType::Background);
	BLACKJACK->deck = CreatorObject<Deck>("Deck", Object::ObjectType::Basic);
	cardrot.Init(BLACKJACK->deck);
	CreatorObject<Button>("Button", Object::ObjectType::Basic, DXMath::Vector3(0, 200, 0), []() {}); //{SCENEMANAGER->ChangeScene("GAMBLE");});
	//CreatorObject<UIButton>("bu", Object::ObjectType::UI, "STAGE1/UI/mybutton2.png", []() {SCENEMANAGER->ChangeScene("GAMBLE");});
	//CreatorObject<TestObj>("Font", Object::ObjectType::UI);

	BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition();

}

void TestScene::Update(const float _deltaTime)
{
	__super::Update(_deltaTime);
	cardrot.Update(_deltaTime, BLACKJACK->deck->cards);
}

void TestScene::ResetInformation()
{
}

void TestScene::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::D1))
	{
		SCENEMANAGER->ChangeScene("DialogScene1");
	}
}