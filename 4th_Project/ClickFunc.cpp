#include "pch.h"
#include "ClickFunc.h"
#include "Card.h"
#include "BlackJack.h"
void ClickFunc::SelectCard()
{
	
}
void ClickFunc::OpenButton()
{
	BLACKJACK->SetState(PlayerState::OPEN);
}

void ClickFunc::StayButton()
{
	BLACKJACK->SetState(PlayerState::STAY);
}

void ClickFunc::HitButton()
{
	BLACKJACK->SetState(PlayerState::HIT);
}

void ClickFunc::DoubleDown()
{
	//더블다운 컷신넣기 *****
	BLACKJACK->onDoubbleDown = true;
}
