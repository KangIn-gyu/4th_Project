#pragma once
#include "../Engine/SingletonBase.h"
#include "../Engine/DirectXInput.h"
#include "../Engine/GameManager.h"
#include "BlackJack.h"


#define MYGAMEMANAGER MyGameManager::GetInstance().get()
class MyGameManager : public GameManager, public SingletonBase<MyGameManager>
{
	MyGameManager() {};
	friend class SingletonBase<MyGameManager>;
public:

	virtual void Update(float _deltaTime) override;

	void UpdateGambleButton();
	void setSkillBtn(bool _state);
	bool CheckClick();
	PlayerState pre;
	bool wasPressed = false;
	bool isBtnOn = false;

};

