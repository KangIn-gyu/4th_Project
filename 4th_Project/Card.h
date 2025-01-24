#pragma once
#include "../Engine/Object.h"
#include "../Engine/IClick.h"
#include "../Engine/IOnmouse.h"
enum class Suit
{
	Spade, 
	Diamond,
	Heart, 
	Clover
};

std::string enumToString(Suit _suit);

class Card : public Object, public IClick, public IOnmouse
{
public:
	Card(std::string_view _name, Object::ObjectType _type, Suit _suit,std::string _rank);
	virtual ~Card() override;
	virtual void Start();
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void Open(); //뒤집어져있다가 오픈할떄 쓸꺼
	int GetValue(); //숫자 계산할때슬 카드 값리턴
	virtual void OnClick() override;
	virtual void OnMouse() override;
	Suit suit;
	std::string rank; //카드 숫자 1~10 J Q K A //
	bool isOpen = false; //카드가 오픈상태인지
private:
	bool isSeleted = false;  //선택된 카드 표시용
	bool AtoOne = true; //Ace를 1로쓸지 11로쓸지 true면 1로
};

