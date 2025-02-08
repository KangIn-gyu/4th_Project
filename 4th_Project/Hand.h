#pragma once
#include "Card.h"
enum class HandState
{
	Base,   //제일 처음
	Open,  //Base상태에서 카드 첫드로 다하고나면 -> Open  해서 3~ 초간 카드보여주고 
	Close,   //보여주고나면 Close해서 다시 뒤집고 
	Center, //뒤집고나면 Center  중앙으로 모으는 
	Shuffle,//셔플연출 필요
	Finish, //연출끝
	
};
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
	void SetState(HandState _state) { curHand = _state; }
private:
	float elapsedTime = 0;
	bool isSenter = false;
	bool endShuffle = false;
	HandState curHand = HandState::Open;
public:
	int maxHand{}; //손패 최대치
	std::vector<Card*> hand;
	
	//핸드의 각위치 정해줘야 하는대 map으로 바꾸기
};

extern std::vector<DXMath::Vector3> playerSlots;
extern DXMath::Vector3 dealerSlots;  //딜러는 첫위치만 있음됨
