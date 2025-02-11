#include "pch.h"
#include "BlackJack.h"
#include "../Engine/FactorySystem.h"
#include "Deck.h"
#include "Card.h"
#include "D2DBaseObj.h"
#include "../Engine/SceneManager.h"

#include "../Engine/ModelComponent.h"
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
	betMoney = 0;
	firstTurn = true;
	curTurn = Turn::player;
	isRoundOver = false;
	onDoubbleDown = false;
	magnification = 1;
	SetState(PlayerState::OPEN);
	ChangeState();
	endBet = false;
	canClick = false;
	dealer->SetSkill();
}


void BlackJack::CheckTurnEnd()
{
	ChangeState();
	if (player->turnEnd == true)   //한 오픈 or HIT시마다 할것들
	{
		dealer->turnCount--;
		if (true == player->CheckGameOver())
		{
			//플레이어가 올오픈이지 확인하는 함수필요
		}
		SetState(PlayerState::OPEN);
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
		betMoney += *player->Bet();
		player->betChip = 1000;
		std::cout << "베팅완료 " << std::endl;
		endBet = true;
		canClick = true;
	}
}

void BlackJack::CalculateChips()
{
	sum =  betMoney * magnification;
}

void BlackJack::Update(float _deltaTime)
{
	player->score = player->GetScore();
	dealer->score = dealer->GetScore();
	//std::cout << dealer->turnCount << std::endl;
	CalculateChips();
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
						if (true == player->CardDraw(deck))
						{
							player->turnEnd = true;
						}
						
					}

				}
				else if (state == PlayerState::Skill)
				{
					canClick = true;
					if(true == player->ActiveSkill())
						SetState(PlayerState::OPEN);
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
				if (true == firstAni)
				{
					firstAni = false;
					dealer->GetComponent<ModelComponent>()->SetAnimation(8); //  TODO : 여기는 애니메이션 보류
				}
			
				if (secondAni == true  && true == dealer->GetComponent<ModelComponent>()->IsAnimationFinished())
				{
					dealer->GetComponent<ModelComponent>()->SetAnimation(5);
					secondAni = false;
				}
			
				if (true == dealer->GetComponent<ModelComponent>()->IsAnimationFinished() && secondAni == false)
				{ // TODO : 애니메이션 시간 
					dealer->GetComponent<ModelComponent>()->SetAnimation(4);
				}

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
					dealer->turnCount++;
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
			RoundStart();
		}
		else if (dealer->chip <= 0)
		{
			//dealer->lose
			RoundStart();
		}
	}
}

void BlackJack::DealerTurn(float _deltaTime)
{
	//std::cout << " 딜러턴입니다 " << std::endl;
	//딜러 다이얼로그 출력  선택지선택
	DSkill dealerState = dealer->GetState();

	switch(dealerState)
	{
	case DSkill::none:
		break;
	case DSkill::reverse:
		dialogs[0]->SetActive(true);
		break;
	case DSkill::meditation:
		dialogs[1]->SetActive(true);
		break;
	case DSkill::skillBan:
		dialogs[2]->SetActive(true);
		break;
	case DSkill::slotBan:
		dialogs[3]->SetActive(true);
		break;
	default:
		break;
	}
}

void BlackJack::CheckVictory(float _deltaTime)
{
	auto DealerWin = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "DealerWin");
	auto PlayerWin = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "PlayerWin");
	auto BetResult = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "BetResult");
	auto BetMag = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "BetMag");
	auto Result = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Result");
	//승패계산
	
	if (dealer->GetScore() >= 22)
	{
		std::cout << "딜러가 22넘었음  " << " ㅇㅇ" << std::endl;
		PlayerWin->SetActive(true);
		BetResult->SetActive(true);
		BetMag->SetActive(true);
		Result->SetActive(true);

	}
	else if (player->score == dealer->GetScore())
	{
		std::cout << " 둘이 비겼음 쇼다운으로 " << " ㅇㅇ" << std::endl;
		ShowDown();   //점수 동일하면 쇼다운페이지로	
	}
	else if (player->score > dealer->GetScore())
	{
		std::cout << "플레이어가 이김 " << " ㅇㅇ" << std::endl;
		//플레이어 윈 연출로
		PlayerWin->SetActive(true);
		BetResult->SetActive(true);
		BetMag->SetActive(true);
		Result->SetActive(true);
	}
	else
	{
		std::cout << "딜러가 이김 " << " ㅇㅇ" << std::endl;
		//딜러윈 연출로
		DealerWin->SetActive(true);
		BetResult->SetActive(true);
		BetMag->SetActive(true);
		Result->SetActive(true);
	}

	
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
	default:
		return "OPEN";
	};
}
