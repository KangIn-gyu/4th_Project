#include "pch.h"
#include "ButtonFunc.h"
#include "Card.h"
#include "GameManager.h"
void ButtonFunc::SelectCard()
{
	
}
void ButtonFunc::CardOpenButton()
{
	GAMEMANAGER->SetState(PlayerState::OPEN);
}

void ButtonFunc::StayButton()
{
	GAMEMANAGER->SetState(PlayerState::STAY);
}

void ButtonFunc::HitButton()
{
	GAMEMANAGER->SetState(PlayerState::HIT);
}

void ButtonFunc::DoubleDown()
{
	//더블다운 컷신넣기 *****
	GAMEMANAGER->SetState(PlayerState::DOBULEDOWN);
}
