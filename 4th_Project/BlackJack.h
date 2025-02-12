#pragma once
#include "../Engine/SingletonBase.h"

#include "Deck.h"
#include "Card.h"

#include "Player.h"
#include "Dealer.h"
#define BLACKJACK BlackJack::GetInstance().get()

class D2DBaseObj;
enum class Turn
{
	player,
	dealer,
	CheckVictory
};
enum class PlayerState   //플레이어 행동상태 
{
	OPEN,    //기본시작은 open
	HIT,
	STAY,
	Skill,    //
};

std::string stateToString(PlayerState _state);
class BlackJack : public SingletonBase<BlackJack>
{
	friend class SingletonBase<BlackJack>;
	BlackJack();
	~BlackJack();
public:
	void SetDialog(D2DBaseObj* dialog) { dialogs.push_back(dialog); }

	void SetResultImage(D2DBaseObj* image) {}

	void Setstage(int num =1);   //스테이지 숫자로 스테이지 설정?
	
	void Update(float _deltaTime);
	
	PlayerState GetState() const { return state; }
	std::string getstatestring() { return stateToString(state); } //실험용 지울거
	void SetState(PlayerState _state) { nextState = _state; } //다음꺼 세팅해주고
	void ChangeState() 
	{
		if (nextState != state) //다음꺼 정한게 지금이랑다르면 스테이트 교체 
		{
			state = nextState;
		}
	};

	void DealerTurn(float _deltaTime); //딜러턴 시작? 딜러 카운트가0이됬을떄 딜러턴끝나면 다시 플레이어턴

	// 칩분배 
	void distribution(bool _playerWin)
	{ 
		if (_playerWin)
		{
			player->chip += sum;
			dealer->chip - +sum;

		}
		else
		{
			player->chip -= sum;
			dealer->chip += sum;
		}
	};
	void CheckVictory(float _deltaTime); 
	void ShowDown();   //승패 계산떄 숫자합이 같으면 발생
	void DoubbleDown();
	void RoundStart(); //라운드 시작시  덱초기화, 플레이어6장주기  
	void CheckTurnEnd();  //플레이어가 행동했는지 확인
	void Bet();
	void CalculateChips();

	void StageWin();
	void StageLose();
	void PlayerWin();
	void DealerWin();
	Deck* deck;
	Deck* trashDeck;
	Player* player;
	Dealer* dealer;
	bool onDoubbleDown = false;
	bool endBet = false;
	bool canClick = false;
	float magnification;
	float maxmagnification = 50;
	bool firstTurn = true; //첫턴은 달라서
	Turn curTurn = Turn::player;
	bool firstBet = true;
	int WinStage = 0;
	int betMoney = 0;
	int curStage = 0; //1스테이지부터?
	int sum = 0;
	int showDownCount = 0;
	bool isRoundOver = true; //한 라운드가 끝날떄
private:
	float elapsedTime =0;

	PlayerState state = PlayerState::OPEN;
	PlayerState nextState;

	//bool canChange = true;

	 //배율   최종 = 베팅액 * 배율
	std::vector<D2DBaseObj*> dialogs;

	bool firstAni = true;
	bool secondAni = true;
	bool IdleAni = false;
};

