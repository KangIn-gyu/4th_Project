#pragma once
#include "Card.h"
#include "../Engine/Object.h"
class Card;

class Deck : public Object
{
public:
	Deck(std::string_view _name, Object::ObjectType _type, bool real =true);

	void Init(); //스테이지 끝나면 카드리필
	Card* DrawCard(bool Dealer);   //덱에서 카드한장씩 뽑아주기 딜러일경우 검정색만
	void ShuffleDeck();
	virtual void Initialize() override;
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	//확인용
	void showDeck();
	std::vector<Card*> cards;
	bool _real;
private:

	
	
	

};

