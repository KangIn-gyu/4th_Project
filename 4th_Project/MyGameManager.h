#pragma once
#include "../Engine/SingletonBase.h"



#define GAMEMANAGER BlackJack::GetInstance().get()
class MyGameManager : public SingletonBase<MyGameManager>
{
	MyGameManager() {};
	virtual ~MyGameManager();
	friend class SingletonBase<MyGameManager>;
public:
	void Update(float _deltaTime);
	//¸Ó³ÖÁö
};

