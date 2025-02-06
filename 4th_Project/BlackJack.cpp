#include "pch.h"
#include "BlackJack.h"
#include "../Engine/FactorySystem.h"
#include "Deck.h"
#include "Card.h"
BlackJack::BlackJack()
{
}

void BlackJack::Setstage(int num)
{
	dealer->SetChip(num * 100);
	RoundStart();
}

void BlackJack::RoundStart()
{
	dealer->Init();
	player->Init();
	deck->Init();
	deck->ShuffleDeck();
	
	firstTurn = true;
	curTurn = Turn::player;
	isRoundOver = false;
	onDoubbleDown = false;
	magnification = 1;
	state = PlayerState::OPEN;
	endBet = false;
	canClick = false;
}

void BlackJack::RoundEnd()
{
	
}
void BlackJack::CheckTurnEnd()
{
	if (player->turnEnd == true)   //한 오픈 or HIT시마다 할것들
	{
		//dealer->turnCount--;
		if (true == player->CheckGameOver())
		{

		}
		player->turnEnd = false;
		player->isDrawOne = false;
		endBet = false;
		canClick = false;
	}
}
void BlackJack::Bet()
{
	if (endBet == false) //베팅이 안끝났으면 베팅하고 베팅끝
	{
		betMoney = player->Bet();
		std::cout << "베팅완료 " << std::endl;
		endBet = true;
		canClick = true;
	}
}
void BlackJack::Update(float _deltaTime)
{
	if (dealer->turnCount <= 0)
		curTurn = Turn::dealer;
	if (isRoundOver == false) //라운드시작
	{
		if (firstTurn == false) //첫턴아닐때 
		{
			if (curTurn == Turn::player)
			{
				CheckTurnEnd();
				if (state == PlayerState::OPEN) 
				{
					//모든카드가 open 상태일경우 처리필요
				}
				else if (state == PlayerState::HIT)
				{
					if (!player->isDrawOne && true == endBet)
					{
						player->CardDraw(deck);
					}

				}
				else if (state == PlayerState::Skill)
				{
					player->ActiveSkill();
				}
				else if (state == PlayerState::STAY)
				{
					curTurn = Turn::CheckVictory; 
				}
			}
			else if (curTurn == Turn::dealer)
			{
				DealerTurn(_deltaTime);
			}
			else if (curTurn == Turn::CheckVictory)
			{
				dealer->CardDraw(deck);
				if (dealer->finishDraw == true)
				{
					CheckVictory(_deltaTime);
				}
			}
		}
		else //첫턴에만 실행할거
		{
			elapsedTime += _deltaTime;
			if ( player->drawFirst == false && elapsedTime >= 1.0)
			{
				player->FirstDraw(deck); //1초에한장 딜레이주기 카드위치선정 ******
				elapsedTime = 0;
			}
			//3초뒤에 플레이어카드  뒤집고 섞는 연출 필요
			if (player->drawFirst == true  && player->Shuffle == false)
			{
				player->ShuffleHand();
				elapsedTime = 0;
				if(player->Shuffle)
					canClick = true;
			}

			//플레이어가 2장 뒤집기 기다리고 뒤집으면 딜러2장주고 한장뒤집기
			if (player->Open2Card() == true)
			{
				canClick = false;
				if (elapsedTime >= 2.0)
				{
					dealer->FirstDraw(deck); // 
					elapsedTime = 0;
				}
				if (dealer->finishFirst == true)
				{
					firstTurn = false;
					player->turnEnd = false;
					//canClick = true;
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

void BlackJack::DealerTurn(float _deltaTime)
{
	
	//딜러 다이얼로그 출력  선택지선택
	dealer->Act();
	std::cout << " 딜러턴입니다 " << std::endl;
	//다이얼로그 패턴 끝나면 curTurn = Turn::player;
}

void BlackJack::CheckVictory(float _deltaTime)
{
	//승패계산
	if (dealer->GetScore() >= 22)
	{
		std::cout << "딜러가 22넘었음  " << " ㅇㅇ" << std::endl;
	}
	else if (player->GetScore() == dealer->GetScore())
	{
		std::cout << " 둘이 비겼음 쇼다운으로 " << " ㅇㅇ" << std::endl;
		ShowDown();   //점수 동일하면 쇼다운페이지로	
	}
	else if (player->GetScore() > dealer->GetScore())
	{
		std::cout << "플레이어가 이김 " << " ㅇㅇ" << std::endl;
		//플레이어 윈 연출로
	}
	else
	{
		std::cout << "딜러가 이김 " << " ㅇㅇ" << std::endl;
		//딜러윈 연출로
	}
	isRoundOver = true;
}

void BlackJack::ShowDown()
{
}

void BlackJack::DoubbleDown()
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
