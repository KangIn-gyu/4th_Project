#include "pch.h"
#include "Player.h"
#include "Hand.h"
 

void Player::Start()
{
	
}



void Player::CardDraw(Deck* _deck)
{
	
	//카드가 7장일경우 처리필요 *****
	if (hand.numCard() < 6)
	{
		hand.cardDraw((_deck->DrawCard()), handSlots[hand.numCard()]);
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
		if (card->isOpen == true)
			count++;
	}
	return (count >= 2);
}

