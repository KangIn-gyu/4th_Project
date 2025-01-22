#include "pch.h"
#include "Deck.h"
#include "Card.h"
#include <algorithm>
#include <random>
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "../Engine/FactorySystem.h"
Deck::Deck(std::string_view _name, Object::ObjectType _type) : Object(_name, _type)
{
	for (Suit suit : { Suit::Spade, Suit::Diamond, Suit::Heart, Suit::Clover }) {
		// 모든 값 순회
		for (std::string rank : { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" }) {
			auto newcard = SCENEMANAGER->currentScene->CreatorObject<Card>((enumToString(suit) + rank), Object::ObjectType::Basic, suit, rank);

			cards.push_back(newcard);
		}
	}
}

Card* Deck::DrawCard(int num)
{
	
	Card* card = cards.back();
	cards.pop_back();
	return card;
}


void Deck::Start()
{
}

void Deck::showDeck()
{
	for (auto& card : cards)
	{
		if (card) { // card가 nullptr인지 확인
			std::cout << card->name << std::endl;
		}
	}
}


