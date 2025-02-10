#include "pch.h"
#include "Hand.h"
#include "Card.h"
#include <algorithm>
#include "../Engine/DOTween.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/TimeSystem.h"
#include "BlackJack.h"
#include "../Engine/Helper.h"

std::vector<DXMath::Vector3> playerSlots = {
	{-40, 65,0}, {-25, 65,0}, {-10, 65,0}, {5, 65,0},
	{20, 65,0}, {35, 65,0}, {50, 65,0}
}; //이건 어쩌지
DXMath::Vector3 dealerSlots = {-20,65.0f,20.0f};
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
		// TODO : 25.2.10 수정함
		// std::random_device rd;
		// std::mt19937 g(rd());
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
