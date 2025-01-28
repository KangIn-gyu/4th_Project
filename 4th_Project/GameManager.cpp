#include "pch.h"
#include "GameManager.h"
#include "../Engine/FactorySystem.h"

#include "Deck.h"
#include "Card.h"
GameManager::GameManager()
{

}

void GameManager::Setstage(int num)
{
	dealer->SepChip(num * 100); //
	firstTurn = true;
	curTurn = Turn::player;
}

void GameManager::Update(float _deltaTime)
{
	if (dealer->turnCount <= 0)
		curTurn = Turn::dealer;
	if (isRoundOver == false) //라운드시작
	{
		if (firstTurn == false) //첫턴아닐때 
		{
			if (curTurn == Turn::player)
			{
				if (state == PlayerState::OPEN || state == PlayerState::HIT)  ///STAY아니면 똑같이 처리
				{
					
				}
				else if (state == PlayerState::STAY)
				{
					curTurn = Turn::CheckVictory; //버튼눌러서 Stay로바꾸게할것
				}
			}
			else if (curTurn == Turn::dealer)
			{
				DealerTurn(_deltaTime);
			}
			else if (curTurn == Turn::CheckVictory)
			{
				CheckVictory(_deltaTime);
			}
		}
		else //첫턴에만 실행할거
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
	}
	else  //라운드끝났으면 다시 라운드시작
	{	
		if(player->chip > 0 && dealer->chip > 0)
			RoundStart(); //둘다 0보다많으면 라운드 다시시작
		else if(player->chip <= 0 )//딜러 플레이어칩 보고 둘중한개가 0이하면 연출후 다음씬으로 
		{
			//player->lose
		}
		else if (dealer->chip <= 0)
		{
			//dealer->lose
		}
	}
}

void GameManager::DealerTurn(float _deltaTime)
{
	
	//딜러 다이얼로그 출력  선택지선택
	dealer->Act();

	//다이얼로그 패턴 끝나면 curTurn = Turn::player;
}

void GameManager::CheckVictory(float _deltaTime)
{
	//승패계산

	if (player->GetScore() == dealer->GetScore())
	{
		ShowDown();   //점수 동일하면 쇼다운페이지로	
	}
	else if (player->GetScore() > dealer->GetScore())
	{
		//플레이어 윈 연출로
	}
	else
	{
		//딜러윈 연출로
	}
}

void GameManager::ShowDown()
{
}

void GameManager::DoubbleDown()
{
}

void GameManager::RoundStart()
{
	deck->Init();
	deck->ShuffleDeck();
	
	isRoundOver = false;
	onDoubbleDown = false;
	magnification = 1;
}

void GameManager::RoundEnd()
{
	
}

std::string stateToString(PlayerState _state)
{
	switch (_state)
	{
	case PlayerState::OPEN:
		return "OPEN";
	case PlayerState::HIT:
		return "HIT";
	case PlayerState::STAY:
		return "STAY";
	case PlayerState::SHOWDOWN:
		return "SHOWDOWN";
	default:
		return "OPEN";
	};
}
