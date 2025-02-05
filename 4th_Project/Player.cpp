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

void Player::ActiveSkill()
{
	skil();
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

void Player::skiil1()
{
	//사용하면 뒷면카드에 테두리 생성카드 클릭들어오면 카드 뒤집히고 2초뒤 다시 뒤집히고 스킬끝
	
	if (selectCard != nullptr)  //카드 선택 완료했으면
	{
		selectCard->Reverse();  //앞면으로 뒤집고
		
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

void Player::skill2()
{
	//최대치 설정해서 제한필요
	BLACKJACK->magnification *= 2;
}

void Player::skill3()
{
	chip *= 1.1f;
}

void Player::skill4()
{
}




