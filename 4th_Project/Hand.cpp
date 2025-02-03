#include "pch.h"
#include "Hand.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include "../Engine/DOTween.h"
#include "../Engine/TransformComponent.h"
#include "BlackJack.h"
void Hand::Init(int num)
{
	maxHand = num;
	for (int i = 0; i < num; i++)
	{
		hand.push_back(nullptr);
	}
}
void Hand::handReset(bool dealer)
{
	
	for (int i = 0; i < numCard(); i++)
	{
		BLACKJACK->trashDeck->cards.push_back(hand[i]);
		hand[i] = nullptr;
	}
	/*if (!dealer)
	{
		for (int i = 0; i < maxHand; i++)
		{
			hand.push_back(nullptr);
		}
	}*/
}

Card* Hand::cardDraw(Card* _card,DXMath::Vector3 _pos, bool dealer)
{
	//_pos로 옮기기
	auto& cardpos = _card->GetComponent<TransformComponent>()->GetPosition();

	if (dealer == false)
	{
		for (int i = 0; i < maxHand; i++)
		{
			if (hand[i] == nullptr)
			{
				hand[i] = _card;
				new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, playerSlots[i].x);
				new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, playerSlots[i].y);
				break;
			}
		}
	}
	else
	{
		new DOTween(cardpos.x, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.x, _pos.x);
		new DOTween(cardpos.y, EasingEffect::OutExpo, StepAnimation::StepOnceForward, 1.f, cardpos.y, _pos.y);
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
			count++;
	}
	return count;
}
