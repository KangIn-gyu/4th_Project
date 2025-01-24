#include "pch.h"
#include "Hand.h"
#include "Card.h"
#include <algorithm>
#include <random>

void Hand::handReset()
{
	hand.clear();
}

void Hand::cardDraw(Card* _card)
{
	hand.push_back(_card);
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
	for (auto card : hand)
	{
		if (card->isOpen == true)
			count++;
	}
	return count;
}
