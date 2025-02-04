#pragma once
#include "../Engine/SingletonBase.h"



#define GAMEMANAGER BlackJack::GetInstance().get()
class GameManager : public SingletonBase<GameManager>
{
	GameManager() {};
	virtual ~GameManager();
	friend class SingletonBase<GameManager>;
public:
	void Update(float _deltaTime);
	//¸Ó³ÖÁö
};

