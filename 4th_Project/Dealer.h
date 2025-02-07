#pragma once
#include "../Engine/Object.h"
#include "../Engine/IClick.h"
#include "../Engine/IOnmouse.h"
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
class Dealer : public Object, public IClick, public IOnmouse
{
public:
	Dealer(std::string_view _name, Object::ObjectType _type);

	virtual ~Dealer() {};
	virtual void Initialize();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {};
	virtual void LateUpdate() {};

	void Init();
	void FirstDraw(Deck* _deck);
	void CardDraw(Deck* _deck); 
	int  GetScore();
	void Act();
	void SetChip(int _num) { chip += _num; }
	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;
	void OpenOne(float _deltaTime);
	int turnCount = 3;     //행동카운트
	Hand hand;
	int chip =1000; //딜러칩 스테이지 시작때 채워줌
	bool finishFirst = false;
	bool finishDraw  = false;
private:
	
	std::function<void(void)> pattern; //딜러 다음패턴 담아둘곳
	int maxScore = 17; //딜러가 카드그만뽑을 상한선
};
