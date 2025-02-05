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
	bool ShuffleHand();
	
private:
	float elapsedTime = 0;
	bool isSenter = false;
	bool endShuffle = false;
public:
	int maxHand; //손패 최대치
	std::vector<Card*> hand;
	//핸드의 각위치 정해줘야 하는대 map으로 바꾸기
};

extern std::vector<DXMath::Vector3> playerSlots;
extern DXMath::Vector3 dealerSlots;  //딜러는 첫위치만 있음됨
