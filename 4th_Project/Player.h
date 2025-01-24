#pragma once
#include "../Engine/Object.h"
#include "../Engine/SingletonBase.h"
#include "Deck.h"
#include "Hand.h"
#include "Card.h"

#define PLAYER Player::GetInstance().get()

class Player : public SingletonBase<Player>
{
	friend class SingletonBase<Player>;

public:
	
	virtual void Start();
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}


	void CardDraw(Deck* _deck);
	int Bet() { return betChip; }
	int GetScore(); //카드합 점수
	bool Open2Card(); //첨에 2개뒤집었는지 확인용
	int   chip = 100; //겜블때 쓸 칩 수,전체 돈
private:
	Player() {};
	Player(std::string_view _name, Object::ObjectType _type);

	float love;//호감도 딜러랑만의 호감
	int   betChip; //베팅떄 걸칩갯수 마우스휠로 조절
public:
	bool turnEnd;
	Hand hand;
	std::vector<Card*> selectCard;
};