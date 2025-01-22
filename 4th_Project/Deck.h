#pragma once
#include "Card.h"
#include "../Engine/Object.h"
class Card;

class Deck : public Object
{
public:
	Deck(std::string_view _name, Object::ObjectType _type);

	Card* DrawCard(int num);
	void ShuffleDeck();
	virtual void Start();
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	//È®ÀÎ¿ë
	void showDeck();
private:

	std::vector<Card*> cards;
	
	

};

