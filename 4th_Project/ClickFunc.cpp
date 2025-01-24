#include "pch.h"
#include "ClickFunc.h"
#include "Card.h"
#include "GameManager.h"
void ClickFunc::SelectCard()
{
	
}
void ClickFunc::OpenButton()
{
	GAMEMANAGER->SetState(PlayerState::OPEN);
}

void ClickFunc::StayButton()
{
	GAMEMANAGER->SetState(PlayerState::STAY);
}

void ClickFunc::HitButton()
{
	GAMEMANAGER->SetState(PlayerState::HIT);
}

void ClickFunc::DoubleDown()
{
	//더블다운 컷신넣기 *****
	GAMEMANAGER->onDoubbleDown = true;
}
