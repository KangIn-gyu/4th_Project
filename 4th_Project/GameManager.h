#pragma once
#include "../Engine/Object.h"
#include "../Engine/SingletonBase.h"

#include "Deck.h"
#include "Card.h"

#include "Player.h"
#include "Dealer.h"
#define GAMEMANAGER GameManager::GetInstance().get()
enum class Turn
{
	player,
	dealer,
	CheckVictory
};
enum class PlayerState   //플레이어 행동상태 
{
	OPEN,
	HIT,
	STAY,
	DOBULEDOWN,
	SHOWDOWN, //쇼다운 필요한가
};
class GameManager : public SingletonBase<GameManager>
{
	friend class SingletonBase<GameManager>;
public:
	GameManager();

	void Setstage(int num);   //스테이지 숫자로 스테이지 설정?
	
	void Update(float _deltaTime);
	
	PlayerState GetState() const { return state; }
	void SetState(PlayerState _state) { state = _state; } //
	void DealerTurn(float _deltaTime);//딜러턴 시작? 딜러 카운트가0이됬을떄 딜러턴끝나면 다시 플레이어턴

	void CheckVictory(float _deltaTime); //일단 만듬 stay누를시 승패결정하기
	void ShowDown();   //승패 계산떄 숫자합이 같으면 발생
	void DoubbleDown();
	void RoundStart(); //라운드 시작시  덱초기화, 플레이어6장주기 
	void RoundEnd();   //각 핸드 초기화 등
	
	Deck* deck;
	Player* player;
	Dealer* dealer;
	bool isThrow = false; //카드 버리기모드 
private:
	float elapsedTime;
	Turn curTurn = Turn::player;
	PlayerState state = PlayerState::OPEN;
	bool firstTurn; //첫턴은 달라서
	bool isRoundOver = true; //한 라운드가 끝날떄
	bool isClicked = false;
	int magnification = 1; //배율   최종 = 베팅액 * 배율
	
};

