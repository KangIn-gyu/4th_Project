#include "pch.h"
#include "Player.h"
#include "Hand.h"
#include <algorithm>
#include <random>

Player::Player()
{
	hand.Init(7);
}

void Player::Initialize()
{
	
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
		hand.cardDraw((_deck->DrawCard()));
	else
		drawFirst = true;
}

void Player::CardDraw(Deck* _deck)
{
	if (hand.numCard() >= 7)
	{
		needDiscard = true;
	}
	else
	{
		needDiscard = false;
		auto card = hand.cardDraw((_deck->DrawCard()));
		card->Open();
		if (card->rank == "A")
			card->OpenA();
		isDrawOne = true;
		turnEnd = true;
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


void Player::ShuffleHand()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(hand.hand.begin(), hand.hand.end(), g);  //셔플연출추가
	Shuffle = true;
}

bool Player::CheckGameOver()
{
	return (GetScore() >= 21);
}




