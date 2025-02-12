#pragma once
#include "../Engine/Object.h"
#include "../Engine/IClick.h"
#include "../Engine/IOnmouse.h"
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
enum class DSkill
{
	none,
	reverse,
	meditation,
	skillBan,
	slotBan
};
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
	bool Act();
	void SetChip(int _num) { chip = _num; }
	int* GetChip() { return &chip; }
	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;
	void OpenOne(float _deltaTime);
	int turnCount = 3;     //행동카운트
	Hand hand;
	int chip =0; //딜러칩 스테이지 시작때 채워줌
	bool finishFirst = false;
	bool finishDraw  = false;

	// 스킬 관련
	bool reverse();
	bool meditation();
	bool skillBan();
	bool slotBan();

	void SetSkill();

	DSkill GetState() { return previousSkill; }
	int score = 0;
private:
	std::function<bool()> pattern; //딜러 다음패턴 담아둘곳
	int maxScore = 17; //딜러가 카드그만뽑을 상한선

	DSkill previousSkill = DSkill::none;	// 기본 값


};
