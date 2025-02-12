#include "pch.h"
#include "Deck.h"
#include "Card.h"
#include <algorithm>

#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "../Engine/FactorySystem.h"
#include "../Engine/TransformComponent.h"
#include "BlackJack.h"

#include "../Engine/Helper.h"
#include "../Engine/SoundSystem.h"

Deck::Deck(std::string_view _name, Object::ObjectType _type,bool real) : Object(_name, _type)
{
	if (real)
	{
		for (Suit suit : { Suit::Spade, Suit::Heart, Suit::Clover }) {
			// 모든 값 순회
			for (std::string rank : { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" }) {
				//auto newcard = SCENEMANAGER->GetCurrentScene()->CreatorObject<Card>((enumToString(suit) + "_" + rank), Object::ObjectType::Basic, suit, rank);
				auto scene = SCENEMANAGER->GetScene("GambleScene");
				auto scene2 = SCENEMANAGER->GetCurrentScene();
				
				if (scene == nullptr)
				{
					int a = 4;
				}
				else
				{
					int a = 5;
				}
				auto newcard = SCENEMANAGER->GetScene("GambleScene")->CreatorObject<Card>((enumToString(suit) + "_" + rank), Object::ObjectType::Basic, suit, rank);
				//auto newcard = SCENEMANAGER->GetCurrentScene()->ObjectCreator<Card>((enumToString(suit) + rank), Object::ObjectType::Basic, suit, "A");
				newcard->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
				cards.push_back(newcard);
			}
		}
		CreateComponent<ModelComponent>("GambleScene/FBX/" + GetName() + ".fbx");
		CreateComponent<RenderComponent>();

		auto randerComponet = GetComponent<RenderComponent>();
		randerComponet->SetShader(ShaderType::VS, "Shaders/StaticVS.hlsl");
		randerComponet->SetShader(ShaderType::PS, "Shaders/PixelShaderPS.hlsl");
		//SetEffect(Object::Effect::OutLine);
	}
}

void Deck::Init()
{
	while (!(BLACKJACK->trashDeck->cards.empty()) )
	{
		if(nullptr != BLACKJACK->trashDeck->cards.back())
			cards.push_back(BLACKJACK->trashDeck->cards.back());
		BLACKJACK->trashDeck->cards.pop_back();
	}

	for (auto card : cards)
	{
		card->Init(GetComponent<TransformComponent>()->GetPosition());
		card->slotActive = true;
		card->RemoveEffect(Object::Effect::OutLine);
		card->RemoveEffect(Object::Effect::Banned);
	}
}

Card* Deck::DrawCard(bool Dealer)
{

	Card* card = nullptr;
	while(true)
	{
		if (!cards.empty())
		{
			card = cards.back();
			cards.pop_back();
			if (true == Dealer)
			{
				if (card->suit == Suit::Diamond || card->suit == Suit::Heart)
				{
					card = nullptr;
				}
				else
					break;
			}
			else
			{

				break;
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

void Deck::ShuffleDeck()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Gather, eSoundChannel::BGM);
	std::shuffle(cards.begin(), cards.end(), RandomUtil::gen);
}


void Deck::Initialize()
{
	
	
}

void Deck::showDeck()
{
	for (auto& card : cards)
	{
		if (card) { // card가 nullptr인지 확인
			std::cout << card->GetName() << std::endl;
		}
	}
}


