#include "pch.h"
#include "Player.h"
#include "Hand.h"
#include <algorithm>
#include <random>
#include "../Engine/TransformComponent.h"
#include "BlackJack.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/SceneManager.h"
#include "../Engine/CameraCompoent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"
Player* Player::g_player = nullptr;

Player::Player(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
	g_player = this;
	hand.Init(7);

	CreateComponent<BoxCollider>();
	DXMath::Vector3 extent = { 10.0f,10.0f,10.f };
	DXMath::Vector3 center = { 0,0,0 };
	GetComponent<BoxCollider>()->SetBox(center, extent, GetComponent<TransformComponent>()->GetQuaternion(),Type::Block);
	GetComponent<BoxCollider>()->SetNotify(this);
	GetComponent<BoxCollider>()->TouchType = Touch::None;
	//GetComponent<TransformComponent>()->SetPosition({ 0,60,-100 });

	
}
void Player::Initialize()
{
	
}

void Player::Update(const float _deltaTime)
{
	Object* camera = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::Camera, 0);
	TransformComponent* cameratrans = camera->GetComponent<TransformComponent>();
	
	GetComponent<TransformComponent>()->SetPosition(cameratrans->GetPosition());
	
	

}


void Player::Init()
{
	turnEnd = false;
	drawFirst = false;
	isDrawOne = false;
	Shuffle = false;
	needDiscard = false;
	openCard = 0; //d
	hand.handReset();
}

void Player::FirstDraw(Deck* _deck)
{
	if (hand.numCard() < 6)
		hand.cardDraw((_deck->DrawCard(false)));
	else
		drawFirst = true;
}

bool Player::CardDraw(Deck* _deck)
{
	if (hand.numCard() >= 7)
	{
		needDiscard = true;
	}
	else
	{
		needDiscard = false;
		auto card = hand.cardDraw((_deck->DrawCard(false))); //카드 이동이랑 뒤집기 연출 맞추기*****
		card->MoveOpen();
		if (card->rank == "A")
			card->OpenA();
		isDrawOne = true;
		return true;
	}
	return false;
}


int Player::GetScore()
{
	return hand.GetScore();
}



bool Player::Open2Card()
{
	int count = 0;
	for (auto& card : hand.hand)
	{
		if (card != nullptr && card->isOpen == true)
			count++;
	}
	return (count >= 2);
}


void Player::ShuffleHand()
{
	
	/*for (int i = 0; i < hand.numCard(); i++)
	{
		hand.hand[i]->GetComponent<TransformComponent>()->SetPosition(playerSlots[i]);
	}*/

	if(true == hand.ShuffleHand())
		Shuffle = true;
}

bool Player::CheckGameOver()
{
	return (GetScore() >= 22);
}

void Player::SetSkill(PSkill _skill)
{
	if (_skill == PSkill::fastEye)
		skill = [this]() { return fastEye(); };
	else if (_skill == PSkill::guts)
		skill = [this]() { return guts(); };
	else if (_skill == PSkill::meditation)
		skill = [this]() { return meditation(); };
	else if (_skill == PSkill::Insurance)
		skill = [this]() { return Insurance(); };
}

bool Player::fastEye()
{

	
	if (selectCard != nullptr)  //카드 선택 완료했으면
	{
		for (auto card : hand.hand)
		{
			if (card != nullptr && card->isOpen == false)
			{
				card->SetOutlineColor({ 0,1,0,0 });
				card->RemoveEffect(Object::Effect::OutLine);

			}
		}
		if (true == selectCard->RevereseSec(3.0f))
		{
			selectCard = nullptr;
			return true;
		}
	}
	else                        //카드선택 전까진
	{
		for (auto card : hand.hand)
		{
			if (card != nullptr && card->isOpen == false)
			{
				//card.is테두리 = ture;
				card->SetOutlineColor({ 1,0,0,0 });
				card->AddEffect(Object::Effect::OutLine);
				//빨간색 테두리 생성bool변수 설정
				

			}
		}
	}

	return false;
}

bool Player::guts()
{
	BLACKJACK->magnification *= 2;
	return true;
}

bool Player::meditation()
{
	chip *= 1.1f;
	return true;
}

bool Player::Insurance()
{
	
	return true;
}


bool Player::ActiveSkill()
{
	return skill(); //스킬 발동이 끝나면 true 리턴
}


void Player::AddInputVec(const DXMath::Vector3& input)
{
	inputVector += input;
	inputVector.Normalize();
}

void Player::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	static int lastWheelDelta = 0;
	const DX::Mouse::State& mouseState = DXINPUT->mouse->GetState();
	int wheelDelta = mouseState.scrollWheelValue;
	if (wheelDelta != lastWheelDelta) {
		if (wheelDelta > lastWheelDelta) {
			std::cout << "마우스 휠업함 " << " ";
			betChip++;
		}
		// 휠이 아래로 굴러갔을 때
		else if (wheelDelta < lastWheelDelta) {
			std::cout << "마우스 휠 다운함 " << " ";
			betChip--;
		}
		lastWheelDelta = wheelDelta;
	}
}


void Player::OnBlock(Collider* _myCol, Collider* _otherCol)
{
	std::cout << "부딪혔음" << std::endl;
}