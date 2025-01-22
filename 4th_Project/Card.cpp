#include "pch.h"
#include "Card.h"
#include "../Engine/RenderComponent.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"



Card::Card(std::string_view _name, Object::ObjectType _type,Suit _suit, std::string _rank) : Object(_name, _type)
{
	suit = _suit;
	rank = _rank;
}

void Card::Start()
{
}

void Card::Open()
{
	
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
