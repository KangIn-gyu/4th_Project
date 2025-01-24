#include "pch.h"
#include "Card.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
#include "Player.h"
#include "../Engine/EventSystem.h"
#include "GameManager.h"


Card::Card(std::string_view _name, Object::ObjectType _type,Suit _suit, std::string _rank) : Object(_name, _type)
{
	suit = _suit;
	rank = _rank;
}

Card::~Card()
{
}

void Card::Start()
{
}

void Card::Open()
{
	isOpen = true;

	//카드 뒤집히는 연출 필요 A일경우 1,11정하는 코드필요 *****
}

int Card::GetValue()
{
	if (rank == "A")
	{
		if (AtoOne)
			return 1;
		else
			return 11;
	}
	if (rank == "J" || rank == "Q" || rank == "K") return 10;

	return std::stoi(rank);
}

void Card::OnClick()
{
	if (GAMEMANAGER->GetState() == PlayerState::OPEN)
	{
		for (auto& card : PLAYER->hand.hand) //
		{
			if (card == this && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
			{
				//PLAYER->selectCard.push_back(this);
				//isSeleted = true;
				Open();
				PLAYER->turnEnd = false;
			}
		}
	}
	else if (GAMEMANAGER->GetState() == PlayerState::HIT)
	{
		for (auto& card : PLAYER->hand.hand) //
		{
			if (card == this && isOpen == false) //누른카드가 패에있고 아직 뒷면이면
			{
				
				//카드제거 추가 *****
				PLAYER->turnEnd = false;
				
			}
		}
	}
	
}

void Card::OnMouse()
{
	std::cout << "현재 마우스가 " << name << " 오브젝트 위에 있습니다" << std::endl;
}

std::string enumToString(Suit _suit)
{
	switch (_suit)
	{
	case Suit::Spade:
		return "Spade";
	case Suit::Diamond:
		return "Diamond";
	case Suit::Heart:
		return "Heart";
	case Suit::Clover:
		return "Clover";
	default:
		return  "Unknown";
	}
}
