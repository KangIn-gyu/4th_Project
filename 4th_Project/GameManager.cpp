#include "pch.h"
#include "GameManager.h"
#include "../Engine/FactorySystem.h"

#include "Deck.h"
#include "Card.h"
GameManager::GameManager()
{

}

void GameManager::stageStart(int num)
{
	dealer->SepChip(num * 100); //
}

void GameManager::Update(float _deltaTime)
{
	if (dealer->turnCount <= 0)
		CheckVictory();
	if (firstTurn == false)
	{
		if (curTurn == Turn::player)
		{
			if (state == PlayerState::OPEN)
			{


			}
			else if (state == PlayerState::HIT)
			{

			}
			else if (state == PlayerState::DOBULEDOWN)
			{

			}
		}
	}
	else
	{
		elapsedTime += _deltaTime;


		if (elapsedTime > 1.0)
		{
			player->CardDraw(deck); //1초에한장 딜레이주기 카드위치선정 ******
			elapsedTime = 0;
		}
		//3초뒤에 플레이어카드  뒤집고 섞는 연출 필요
		if (player->hand.hand.size() >= 6 && elapsedTime >= 3.0f)
			player->hand.handShuffle();

		//플레이어가 2장 뒤집기 기다리고 뒤집으면 딜러2장주고 한장뒤집기
		if (player->Open2Card() == true)
		{
			if (elapsedTime > 1.0)
			{
				dealer->CardDraw(deck); //1초에한장 딜레이주기 카드위치선정 ******
				elapsedTime = 0;
			}
			if (dealer->FinishFirst() == true)
			{
				firstTurn = false;
			}

		}
	}

	if (curTurn == Turn::dealer)
	{
		DealerTurn();
	}
	
}



void GameManager::DealerTurn()
{
	
	//딜러 다이얼로그 출력  선택지선택
	dealer->Act();

	curTurn = Turn::player;
}

void GameManager::CheckVictory()
{
}

void GameManager::RoundStart()
{
	deck->Init();
	deck->ShuffleDeck();
	


}

void GameManager::RoundEnd()
{
}
