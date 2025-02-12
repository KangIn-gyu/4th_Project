#include "pch.h"
#include "Player.h"
#include "Hand.h"
#include <algorithm>
#include <random>
#include "../Engine/TransformComponent.h"
#include "BlackJack.h"
#include "../Engine/SceneManager.h"
#include "../Engine/CameraCompoent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/TimeSystem.h"
#include "../Engine/RayCollier.h"
#include "../Engine/CircleCollider.h"
#include "../Engine/BoxCollider.h"
Player* Player::g_player = nullptr;

Player::Player(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
	g_player = this;
	hand.Init(7);

	CreateComponent<CircleCollider>();
	float raduis = 100.0f;
	DXMath::Vector3 center = { 0,0,0 };
	GetComponent<CircleCollider>()->SetCircle(center, raduis, ActiveType::Block);
	GetComponent<CircleCollider>()->SetNotify(this);
	GetComponent<CircleCollider>()->TouchType = Touch::None;
	//GetComponent<TransformComponent>()->SetPosition({ 0,60,-100 });
	CreateComponent<RayCollier>();
	GetComponent<RayCollier>()->SetRay(230.0f);
	GetComponent<RayCollier>()->SetNotify(this);

	//GetComponent<TransformComponent>()->GetPosition();
	
	
}
void Player::Initialize()
{
	Object* camera = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::Camera, 0);
	cameraTransform = camera->GetComponent<TransformComponent>();
}
void Player::ResetInformation()
{
	
}

void Player::Update(const float _deltaTime)
{
	
	
	GetComponent<TransformComponent>()->SetPosition(cameraTransform->GetPosition());
	GetComponent<TransformComponent>()->SetQuaternion(cameraTransform->GetQuaternion());

}


void Player::Init()
{
	turnEnd = false;
	drawFirst = false;
	isDrawOne = false;
	Shuffle = false;
	needDiscard = false;
	canSkill = true;
	openCard = 0; //
	skillPoint = 10;
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

	if (MaxCardOpen() == true)
	{
		if (GetScore() >= 22)
			return true;
	}
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
	fasteye = true;
	
	if (AllCurCardOpen() == true)
	{
		skillPoint += 3;
		selectCard = nullptr;
		fasteye = false;
		OnSkill = false;
		return true;
	}
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
			fasteye = false;
			OnSkill = false;
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
	if (BLACKJACK->magnification >= BLACKJACK->maxmagnification)
	{
		BLACKJACK->magnification = BLACKJACK->maxmagnification;
	}
	OnSkill = false;
	return true;
}

bool Player::meditation()
{
	chip *= 1.1f;
	OnSkill = false;
	return true;
}

bool Player::Insurance()
{
	useRot = true;

	if (false == isRotTrash)
	{
		//한개 버려라
		for (auto card : hand.hand)
		{
			if(card != nullptr)
				card->AddEffect(Object::Effect::OutLine);
			//고르3버릴거 
		}
	}
	else
	{
		for (auto card : hand.hand)
		{
			if (card != nullptr)
				card->RemoveEffect(Object::Effect::OutLine);
			//고르3버릴거 
		}

		if(selectCard == nullptr)
		{
			if (cardrot.isInit == false)
			{
				cardrot.Init(BLACKJACK->deck);
				cardrot.isInit = true;
			}

			cardrot.Update(TIMESYSTEM.get()->GetFloatDeltaTime(), BLACKJACK->deck->cards);
		}
		else
		{
			for (auto card : cardrot.RotCards) //카드 cardrot 안에서하게 수정 *****
			{
				if (card != nullptr)
				{
					card->GetComponent<TransformComponent>()->SetPosition(BLACKJACK->deck->GetComponent<TransformComponent>()->GetPosition());
					float eulerAngle = DirectX::XMConvertToRadians(180);
					DXMath::Quaternion eulerToQuaternion = DXMath::Quaternion::CreateFromYawPitchRoll(0.f, eulerAngle, 0.f);
					card->GetComponent<TransformComponent>()->SetQuaternion(eulerToQuaternion);
				}
			}
			
			hand.cardDraw(selectCard);
			selectCard->MoveOpen();
			hand.SkillDraw(BLACKJACK->deck, selectCard->GetName());
			useRot = false;
			selectCard = nullptr;
			OnSkill = false;
			return true;
		}
	}

	return false;
}

bool Player::AllCurCardOpen()
{
	int count = 0;
	for (auto card : hand.hand)
	{
		if (card != nullptr && card->isOpen == true)
			count++;
	}
	if(count == hand.numCard())
		return true;
}

bool Player::MaxCardOpen()
{
	int count = 0;
	for (auto card : hand.hand)
	{
		if (card != nullptr && card->isOpen == true)
			count++;
	}
	if (count == hand.maxHand)
		return true;
}



bool Player::ActiveSkill()
{
	return skill(); //스킬 발동이 끝나면 true 리턴
}


void Player::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
	static int lastWheelDelta = 0;
	const DX::Mouse::State& mouseState = DXINPUT->mouse->GetState();
	int wheelDelta = mouseState.scrollWheelValue;
	if (BLACKJACK->endBet == false && BLACKJACK->firstTurn == false)
	{
		if (wheelDelta != lastWheelDelta) {
			if (wheelDelta > lastWheelDelta) {
				std::cout << "마우스 휠업함 " << " ";
				betChip += 100;

			}
			// 휠이 아래로 굴러갔을 때
			else if (wheelDelta < lastWheelDelta) {
				std::cout << "마우스 휠 다운함 " << " ";
				betChip -= 100;
				if (betChip <= minBet)
					betChip = minBet;
			}
			lastWheelDelta = wheelDelta;
		}
	}
	if (SCENEMANAGER->GetCurrentScene()->GetName() == "LobbyScene")
	{

		if (_KeyState.IsKeyDown(DirectX::Keyboard::Keys::LeftControl)) //*****
		{
			DXINPUT->mouse->SetMode(DX::Mouse::MODE_ABSOLUTE);
			DXINPUT->mouse->SetVisible(true);
		}
		else
		{
			DXINPUT->mouse->SetMode(DX::Mouse::MODE_RELATIVE);
			DXINPUT->mouse->SetVisible(false);
		}
	}
}




void Player::OnBlock(Collider* _myCol, Collider* _otherCol)
{
	if (SCENEMANAGER->GetCurrentScene()->GetName() == "LobbyScene")
	{
		std::cout << "부딪혔음" << std::endl;


		DirectX::BoundingOrientedBox otherBox = dynamic_cast<BoxCollider*>(_otherCol)->obBox;


		CircleCollider* circleCol = dynamic_cast<CircleCollider*>(_myCol);
		BoxCollider* boxCol = dynamic_cast<BoxCollider*>(_otherCol);

		if (!circleCol || !boxCol) return; // 캐스팅 실패 시 리턴

		// OBB 정보 가져오기
		DirectX::SimpleMath::Vector3 boxCenter = boxCol->obBox.Center;
		DirectX::SimpleMath::Vector3 boxExtents = boxCol->obBox.Extents;
		DirectX::SimpleMath::Quaternion boxRotation = boxCol->obBox.Orientation;

		// 원의 중심과 반지름
		DirectX::SimpleMath::Vector3 circleCenter = circleCol->Circle.Center;
		float circleRadius = circleCol->Circle.Radius;

		// OBB의 축을 구함 (로컬 -> 월드 변환)
		DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(boxRotation);
		DirectX::SimpleMath::Vector3 right = rotationMatrix.Right();    // X축
		DirectX::SimpleMath::Vector3 up = rotationMatrix.Up();          // Y축
		DirectX::SimpleMath::Vector3 forward = rotationMatrix.Forward();// Z축

		// 원의 중심에서 OBB 중심으로 향하는 벡터
		DirectX::SimpleMath::Vector3 localVec = circleCenter - boxCenter;

		// OBB 좌표계에서 가장 가까운 점을 찾기 위해 각 축에 대해 클램핑
		DirectX::SimpleMath::Vector3 closestPoint = boxCenter;

		closestPoint += (std::clamp)(localVec.Dot(right), -boxExtents.x, boxExtents.x) * right;
		closestPoint += (std::clamp)(localVec.Dot(up), -boxExtents.y, boxExtents.y) * up;
		closestPoint += (std::clamp)(localVec.Dot(forward), -boxExtents.z, boxExtents.z) * forward;

		// 원의 중심과 가장 가까운 점 사이의 거리 벡터 구하기
		DirectX::SimpleMath::Vector3 pushDir = circleCenter - closestPoint;

		float dist = pushDir.Length();

		// 침투 여부 확인
		if (dist < circleRadius)
		{
			// 침투 깊이 계산
			float penetrationDepth = circleRadius - dist;

			// 정규화된 방향 벡터
			pushDir.Normalize();

			// 침투 깊이만큼 되돌리기

			DXMath::Vector3 pos = pushDir * penetrationDepth;

			// 원의 새로운 위치 적용


			cameraTransform->SetPosition(cameraTransform->GetPosition() + pos);
			GetComponent<TransformComponent>()->SetPosition(cameraTransform->GetPosition());

		}
	}
}




void Player::EnterRayCollision(Collider* _otherCol)
{
	if (SCENEMANAGER->GetCurrentScene()->GetName() == "LobbyScene")
	{
		if (_otherCol->GetOwner()->GetName() == "Dealer")
		{
			SCENEMANAGER->GetCurrentScene()->GetGameObject(ObjectType::UI, "Handfaster_ToolTip")->SetActive(true);
			std::cout << _otherCol->GetOwner()->GetName() + " 쳐다보는중임" << std::endl;
		}
	}
	
}
void Player::EndRayCollision(Collider* _otherCol)
{
	if (SCENEMANAGER->GetCurrentScene()->GetName() == "LobbyScene")
	{
		if (_otherCol->GetOwner()->GetName() == "Dealer")
		{
			SCENEMANAGER->GetCurrentScene()->GetGameObject(ObjectType::UI, "Handfaster_ToolTip")->SetActive(false);
		}
	}
}