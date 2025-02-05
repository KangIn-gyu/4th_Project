#include "pch.h"
#include "Player.h"
#include "Hand.h"
#include <algorithm>
#include <random>
#include "../Engine/TransformComponent.h"
#include "BlackJack.h"

Player::Player()
{
	hand.Init(7);
}

void Player::Initialize()
{
	
}

void Player::Update(const float _deltaTime)
{
	//std::cout << hand.numCard();
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
		hand.cardDraw((_deck->DrawCard(false)));
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
		auto card = hand.cardDraw((_deck->DrawCard(false)));
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
	
	/*for (int i = 0; i < hand.numCard(); i++)
	{
		hand.hand[i]->GetComponent<TransformComponent>()->SetPosition(playerSlots[i]);
	}*/

	if(true == hand.ShuffleHand())
		Shuffle = true;
}

bool Player::CheckGameOver()
{
	return (GetScore() >= 22);
}

void Player::SetSkill(PSkill _skill)
{
	if (_skill == PSkill::fastEye)
		skill = [this]() { fastEye(); };
	else if(_skill == PSkill::guts)
		skill = [this]() { guts(); };
	else if (_skill == PSkill::meditation)
		skill = [this]() { meditation(); };
	else if(_skill == PSkill::Insurance)
		skill = [this]() { Insurance(); };
}

void Player::fastEye()
{
	if (selectCard != nullptr)  //카드 선택 완료했으면
	{
		if (true == selectCard->RevereseSec(3.0f))
		{

		}


	}
	else                        //카드선택 전까진
	{
		for (auto card : hand.hand)
		{
			if (card != nullptr && card->isOpen == false)
			{
				//빨간색 테두리 생성bool변수 설정

			}
		}
	}
}

void Player::guts()
{
	BLACKJACK->magnification *= 2;
}

void Player::meditation()
{
	chip *= 1.1f;
}

void Player::Insurance()
{
}



bool Player::ActiveSkill()
{
	skill();

	return true;
}



