#pragma once
#include "Card.h"
class Hand
{
public:
	void Init(int num);
	void handReset(bool dealer = false); //
	Card* cardDraw(Card* _card, DXMath::Vector3 _pos = {0,0,0}, bool dealer = false);
	int GetScore(); //카드합 점수
	int numOpenCard();
	int numCard();
private:
	std::vector<DXMath::Vector3> playerSlots = {
	{100, 0,0}, {200, 0,0}, {300, 0,0}, {400, 0,0},
	{500, 0,0}, {600, 0,0}, {700, 0,0}
	}; //이건 어쩌지
public:
	int maxHand; //손패 최대치
	std::vector<Card*> hand;
	//핸드의 각위치 정해줘야 하는대 map으로 바꾸기
};

