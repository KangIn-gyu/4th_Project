#include "pch.h"
#include "Player.h"
#include "Hand.h"
 

void Player::Initialize()
{
	
}



void Player::CardDraw(Deck* _deck)
{
	
	//카드가 7장일경우 처리필요 *****
	if (hand.numCard() < 6)
	{
		hand.cardDraw((_deck->DrawCard()), { 0,0,0 });
	}
	else
	{
		drawFirst = true;
	}
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

void Player::HandClear()
{
	
}

Player::Player()
{
	hand.Init(7);
}

