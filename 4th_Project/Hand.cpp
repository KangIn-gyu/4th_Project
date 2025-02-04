#include "pch.h"
#include "Hand.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include "../Engine/DOTween.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/TimeSystem.h"
#include "BlackJack.h"

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
	if(dealer)
	{
		while(!hand.empty())
		{
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
	if (elapsedTime >= 3.0f && false == isSenter )
	{
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(hand.begin(), hand.end() - 1, g);  //셔플연출추가 셔플이문제였네
		for (int i = 0; i < numCard(); i++)
		{
			auto& cardpos = hand[i]->GetComponent<TransformComponent>()->GetPosition();
			//hand[i] = _card;
			new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, playerSlots[3].x);
			new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, playerSlots[3].y);
		}
		elapsedTime = 0;
		isSenter = true;
	}


	if (elapsedTime >= 1.0f && true == isSenter && false == endShuffle )
	{
		for (int i = 0; i < numCard(); i++)
		{
			auto& cardpos = hand[i]->GetComponent<TransformComponent>()->GetPosition();
			//hand[i] = _card;
			new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, playerSlots[i].x);
			new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, playerSlots[i].y);
		}
		endShuffle = true;
		return true;
	}

	return false;
}
