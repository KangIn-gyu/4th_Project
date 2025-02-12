#include "pch.h"
#include "Hand.h"
#include "Card.h"
#include <algorithm>
#include "../Engine/DOTween.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/TimeSystem.h"
#include "BlackJack.h"
#include "../Engine/Helper.h"
#include "../Engine/SoundSystem.h"

std::vector<DXMath::Vector3> playerSlots = {
	{-40, 105,-500}, {-25, 105,-500}, {-10, 105,-500}, {5, 105, -500},
	{20, 105,-500}, {35, 105,-500}, {50, 105,-500}
}; //이건 어쩌지
DXMath::Vector3 dealerSlots = { -20, 105.f, -480.0f};
DXMath::Vector3 showpslot =   { -40, 105.f, -500.0f };
DXMath::Vector3 showdslot =   { -40, 105.f, -480.0f };
void Hand::Init(int num)
{
	maxHand = num;
	hand.resize(maxHand, nullptr);
}
void Hand::handReset(bool dealer)
{
	elapsedTime = 0;
	isSenter = false;
	endShuffle = false;
	curHand = HandState::Open;
	if(dealer)
	{
		while(!hand.empty())
		{
			if(hand.back() !=nullptr) 
				BLACKJACK->trashDeck->cards.push_back(hand.back());
			hand.pop_back();
		}
	}
	else 
	{
		for (int i = 0; i < maxHand; i++)
		{
			if (hand[i] != nullptr)
				BLACKJACK->trashDeck->cards.push_back(hand[i]);
			hand[i] = nullptr;
		}
	}
	
}

Card* Hand::cardDraw(Card* _card,DXMath::Vector3 _pos, bool dealer)
{
	//_pos로 옮기기

	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Pick, eSoundChannel::Effect);
	auto& cardpos = _card->GetComponent<TransformComponent>()->GetPosition();

	if (false == dealer)
	{
		for (int i = 0; i < maxHand; i++)
		{
			if (hand[i] == nullptr)
			{
				hand[i] = _card;
				new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, playerSlots[i].x);
				new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, playerSlots[i].y);
				new DOTween(cardpos.z, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.z, playerSlots[i].z);
				break;
			}
		}
	}
	else
	{
		new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, _pos.x);
		new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, _pos.y);
		new DOTween(cardpos.z, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.z, _pos.z);
		hand.push_back(_card);
	}
	return _card;
	
}


int Hand::GetScore()
{
	int score = 0;
	if (!hand.empty())
	{
		for (auto& card : hand)
		{
			if (card != nullptr && card->isOpen == true) //null로 손패갯수 관리해서 계속확인팔요
			{
				if (card->suit == Suit::Diamond || card->suit == Suit::Heart)
				{
					score -= card->GetValue();
				}
				else
				{
					score += card->GetValue();
				}
			}
		}
	}
	else
		score = 0;

	return score;
}

int Hand::numOpenCard()
{
	int count = 0;
	for (auto& card : hand)
	{
		if (card->isOpen == true)
			count++;
	}
	return count;
}

int Hand::numCard()
{
	int count = 0;
	for (auto card : hand)
	{
		if (card != nullptr)
		{
			count++;
		}
	}
	return count;
}

bool Hand::ShuffleHand()
{
	float delta = TIMESYSTEM.get()->GetFloatDeltaTime();
	elapsedTime += delta;

	if (curHand == HandState::Open && elapsedTime >= 1.0f) //카드다받았으면 오픈하기
	{

		for (int i = 0; i < numCard(); i++)
		{
			hand[i]->Reverse();
		}
		elapsedTime = 0;
		SetState(HandState::Close);
	}
	
	if (curHand == HandState::Close && elapsedTime >= 2.0f) //일정시간후 다시 닫기
	{

		for (int i = 0; i < numCard(); i++)
		{
			hand[i]->Reverse();
		}
		elapsedTime = 0;
		SetState(HandState::Center);
	}
	if (curHand == HandState::Center && elapsedTime >= 3.0f )  // 중앙으로 모으기 모으면서 카드 값이미 바뀜
	{
		std::shuffle(hand.begin(), hand.end() - 1, RandomUtil::gen);
		for (int i = 0; i < numCard(); i++)
		{
			auto& cardpos = hand[i]->GetComponent<TransformComponent>()->GetPosition();
			//hand[i] = _card;
			new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, playerSlots[3].x);
			new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, playerSlots[3].y);
		}
		elapsedTime = 0;
		SetState(HandState::Shuffle);
	}

	if (curHand == HandState::Shuffle && elapsedTime >= 1.0f  )   //모은카드 펼치면서 위치 재선정
	{
		for (int i = 0; i < numCard(); i++)
		{
			auto& cardpos = hand[i]->GetComponent<TransformComponent>()->GetPosition();
			//hand[i] = _card;
			new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, playerSlots[i].x);
			new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, playerSlots[i].y);
		}
		elapsedTime = 0;
		SetState(HandState::Finish);
		
	}

	return (curHand == HandState::Finish);
}

void Hand::SkillDraw(Deck* deck, std::string _name)
{
	
	auto it = std::remove_if(deck->cards.begin(), deck->cards.end(),
		[&](Card* card) {
			if (card->GetName() == _name) {
				BLACKJACK->trashDeck->cards.push_back(card); // 먼저 trashDeck에 추가
				return true; // 삭제 대상
			}
			return false; // 유지 대상
		}
	);

	deck->cards.erase(it, deck->cards.end()); // 실제 삭제
		
}
