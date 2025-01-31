#pragma once
#include "Card.h"
class Hand
{
public:
	void handReset(); //
	void cardDraw(Card* _card,DXMath::Vector3 _pos = { 0,0,0 });
	void handShuffle();
	int GetScore(); //카드합 점수
	int numOpenCard();
	int numCard() { return hand.size(); }
private:
	
public:
	std::vector<Card*> hand;

	//핸드의 각위치 정해줘야 하는대 
};

