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

}

void Deck::Init()
{
	cards.clear();
	for (Suit suit : { Suit::Spade, Suit::Diamond, Suit::Heart, Suit::Clover }) {
		// 모든 값 순회
		for (std::string rank : { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" }) {
			auto newcard = SCENEMANAGER->currentScene->CreatorObject<Card>((enumToString(suit) + rank), Object::ObjectType::Basic, suit, rank);

			cards.push_back(newcard);
		}
	}
}

Card* Deck::DrawCard(bool Dealer)
{

	Card* card = nullptr;
	while(card == nullptr)
	{
		if (!cards.empty())
		{
			card = cards.back();
			cards.pop_back();
			if (Dealer)
			{
				if (card->suit == Suit::Diamond || card->suit == Suit::Heart)
					card = nullptr;
			}
		}
		else
		{
			std::cout << "덱이 비었습니다 덱이비어서 터짐 " << std::endl;
			return card = new Card("LAST", Object::ObjectType::Basic, Suit::Spade, "0");
		}

	}

	return card;
}

void Deck::ShuffleDeck() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(cards.begin(), cards.end(), g);
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


