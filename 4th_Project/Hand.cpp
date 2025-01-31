#include "pch.h"
#include "Hand.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include "../Engine/DOTween.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "../Engine/FactorySystem.h"
void Hand::Init(int num)
{
	maxHand = num;
	for (int i = 0; i < num; i++)
	{
		hand.push_back(nullptr);
	}
}
void Hand::handReset()
{
	hand.clear();
}

void Hand::cardDraw(Card* _card,DXMath::Vector3 _pos, bool dealer)
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
	
	
}

void Hand::handShuffle()
{
	
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(hand.begin(), hand.end(), g);

	
}

int Hand::GetScore()
{
	int score = 0;
	if (!hand.empty())
	{
		for (auto& card : hand)
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
