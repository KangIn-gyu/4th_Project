#include "pch.h"
#include "BlackJack.h"
#include "../Engine/FactorySystem.h"
#include "Deck.h"
#include "Card.h"
#include "D2DBaseObj.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ModelComponent.h"
#include "../Engine/Helper.h"
#include "../Engine/SoundSystem.h"

BlackJack::BlackJack()
{
}

BlackJack::~BlackJack()
{

}

void BlackJack::Setstage(int num)
{
	curStage = num;
	isGameOver = false;
	dealer->SetChip(num * 100);
	RoundStart();
}
void BlackJack::RoundStart()
{
	canClick = false;
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
	dealer->SetSkill();
	firstBet = true;

	player->canSkill = true;

	for (auto card : player->hand.hand)
	{
		if (card != nullptr)
			card->slotActive = true;
	}
}


void BlackJack::CheckTurnEnd()
{

	ChangeState();
	if (player->turnEnd == true)   //한 오픈 or HIT시마다 할것들
	{
		dealer->turnCount--;
		player->SpPlus();
		if (true == player->CheckGameOver())
		{
			//플레이어가 올오픈이지 확인하는 함수필요
			//DealerWin();
			curTurn = Turn::CheckVictory;
		}
		SetState(PlayerState::OPEN);
		player->turnEnd = false;
		player->isDrawOne = false;
		endBet = false;
		canClick = false;
		player->canSkill = true;

		for(auto card : player->hand.hand)
		{
			if(card != nullptr)
				card->slotActive = true;
		}
	}
	if (player->MaxCardOpen() == true) //다 오픈이면
	{

		curTurn = Turn::CheckVictory;
	}
}

void BlackJack::Bet()
{
	if (endBet == false) //베팅이 안끝났으면 베팅하고 베팅끝
	{
		firstBet = false;
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

void BlackJack::StageWin()
{

}

void BlackJack::StageLose()
{
	//스테이지지면?
	isGameOver = true;
	SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "RE Start_Button")->SetActive(true);
	SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "ToTitle_Button")->SetActive(true);
}

void BlackJack::PlayerWin()
{
	IsShowDown = true;
	firstAni = true;
	auto DealerWin = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "DealerWin");
	auto PlayerWin = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "PlayerWin");
	auto BetResult = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "BetResult");
	auto BetMag = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "BetMag");
	auto Result = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Result");


	PlayerWin->SetActive(true);
	BetResult->SetActive(true);
	BetMag->SetActive(true);
	Result->SetActive(true);
	
	// SceneManager().changer(DialogScene4)
		//  static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("DialogScene4");
}

void BlackJack::DealerWin()
{
	IsShowDown = true;
	firstAni = true;

	auto DealerWin = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "DealerWin");
	auto PlayerWin = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "PlayerWin");
	auto BetResult = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "BetResult");
	auto BetMag = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "BetMag");
	auto Result = SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Result");


	DealerWin->SetActive(true);
	BetResult->SetActive(true);
	BetMag->SetActive(true);
	Result->SetActive(true);
	//재도전 버튼 and 로비로 버튼 뛰우기
	
	SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
	SOUNDSYSTEM->PlayMusic(eSoundList::GameOver, eSoundChannel::BGM);
}

void BlackJack::Update(float _deltaTime)
{
	if (isGameOver == false)
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
						if (true == player->ActiveSkill())
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

					if (true == player->CheckGameOver())
					{
						DealerWin();
					}
					else
					{
						dealer->CardDraw(deck);
						if (dealer->finishDraw == true)
						{
							CheckVictory(_deltaTime);
						}
					}
				}
			}
			else //첫턴에만 실행할거
			{
				elapsedTime += _deltaTime;

				if (player->drawFirst == false && elapsedTime >= 2.3)
				{
					if (true == firstAni)  // TODO : 애니메이션 처리
					{
						IdleAni = false;
						firstAni = false;
						dealer->GetComponent<ModelComponent>()->SetAnimation(8); //  TODO : 여기는 애니메이션 보류
					}
					if (secondAni == true && true == dealer->GetComponent<ModelComponent>()->IsAnimationFinished())
					{
						SOUNDSYSTEM->PlayMusic(eSoundList::SE_Card_Unfold, eSoundChannel::Effect);
						dealer->GetComponent<ModelComponent>()->SetAnimation(5);
						secondAni = false;
					}
					if (true == dealer->GetComponent<ModelComponent>()->IsAnimationFinished() && secondAni == false)
					{
						dealer->GetComponent<ModelComponent>()->SetAnimation(4);
						IdleAni = true;
					}

					player->FirstDraw(deck); //1초에한장 딜레이주기 카드위치선정 ******
					elapsedTime = 0;
				}
				//3초뒤에 플레이어카드  뒤집고 섞는 연출 필요
				if (player->drawFirst == true && player->Shuffle == false)
				{
					player->ShuffleHand();
					elapsedTime = 0;
					if (player->Shuffle)
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
						player->skillPoint--;
					}

				}
			}
		}
		else  //라운드끝났으면 다시 라운드시작
		{
			if (player->chip > 0 && dealer->chip > 0)
				RoundStart(); //둘다 0보다많으면 라운드 다시시작
			else if (player->chip <= 0)//딜러 플레이어칩 보고 둘중한개가 0이하면 연출후 다음씬으로 
			{
				StageWin();
				//RoundStart();
			}
			else if (dealer->chip <= 0)
			{
				StageLose();
			}
		}

		if (true == IdleAni && dealer->GetComponent<ModelComponent>()->IsAnimationFinished())
		{
			dealer->GetComponent<ModelComponent>()->SetAnimation(1);
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

	//승패계산
	

	if (dealer->GetScore() >= 22)
	{
		SOUNDSYSTEM->PlayMusic(eSoundList::VS_Blackjack, eSoundChannel::Voice);
		PlayerWin();
	}
	else if (player->score == dealer->GetScore())
	{
		ShowDown();   //점수 동일하면 쇼다운페이지로	
	}
	else if (player->score > dealer->GetScore() && player->score <= 21)
	{
		//플레이어 윈 연출로
		PlayerWin();
	}
	else
	{
		std::cout << "딜러가 이김 " << " ㅇㅇ" << std::endl;
		//딜러윈 연출로
		DealerWin();
	}

	
}


void BlackJack::ShowDown()
{
	SCENEMANAGER->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "ShowDownImage")->SetActive(true);
	//컷씬 뛰우고
	//배율 X2 최대치 제한있는지 확인
	//카드 한장씩 뽑기-> 동점일경우 계속
	player->hand.handReset();
	dealer->hand.handReset();  //각핸드 리셋하고
	
	if (true == IsShowDown)
	{
		IsShowDown = false;
		SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
		SOUNDSYSTEM->PlayMusic(eSoundList::DoubleDown, eSoundChannel::BGM);
	}

	if (player->hand.GetScore() == dealer->hand.GetScore())  //다를떄까지 반복
	{
		player->hand.cardDraw(deck->DrawCard(false), { showpslot.x + showDownCount * 15.0f,showpslot.y, showpslot.z }, true);
		dealer->hand.cardDraw(deck->DrawCard(true), { showdslot.x + showDownCount * 15.0f,showdslot.y, showdslot.z }, true);
		showDownCount++;
	}
	else //다르면 승페계싼
	{
		if (player->hand.GetScore() < dealer->hand.GetScore()) //작은쪽이 이기는거
		{
			SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
			SOUNDSYSTEM->PlayMusic(eSoundList::GameScene, eSoundChannel::BGM);
			SOUNDSYSTEM->PlayMusic(eSoundList::VS_Win, eSoundChannel::Voice);
			PlayerWin();
		}
		else
		{
			SOUNDSYSTEM->PlayMusic(eSoundList::GameScene, eSoundChannel::BGM);
			SOUNDSYSTEM->PlayMusic(eSoundList::VS_Lose, eSoundChannel::Voice);
			DealerWin(); //딜러가이김
		}
	}	
	
}

void BlackJack::DoubbleDown()
{
	SOUNDSYSTEM->StopMusic(eSoundChannel::BGM);
	SOUNDSYSTEM->PlayMusic(eSoundList::DoubleDown, eSoundChannel::BGM);

	if (BLACKJACK->onDoubbleDown == false)
	{
		BLACKJACK->magnification *= 2;
		if (BLACKJACK->magnification >= BLACKJACK->maxmagnification)
		{
			BLACKJACK->magnification = BLACKJACK->maxmagnification;
		}
		BLACKJACK->onDoubbleDown = true;
	}
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
