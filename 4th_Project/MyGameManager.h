#pragma once
#include "../Engine/SingletonBase.h"
#include "../Engine/DirectXInput.h"


#define MYGAMEMANAGER MyGameManager::GetInstance().get()
class MyGameManager : public SingletonBase<MyGameManager>
{
	friend class SingletonBase<MyGameManager>;

	MyGameManager() {};
	virtual ~MyGameManager();
	friend class SingletonBase<MyGameManager>;
public:
	

	void Update(float _deltaTime);

	void setSkillBtn(bool _state);
	bool CheckClick();
	//¸Ó³ÖÁö
	bool wasPressed = false;
	bool isBtnOn = false;
};

