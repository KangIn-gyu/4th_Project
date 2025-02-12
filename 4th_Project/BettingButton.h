#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"
#include "../Engine/IOnmouse.h"
#include "GambleButton.h"

class BoxCollider;
class D2DRenderComponent;


class BettingButton :public Object, public IClick, public IOnmouse
{
public:
	BettingButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func);
	~BettingButton();

	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void Blink();
	void ChangeBit();
	void ChangeState(gbState _state);//그림바꾸기용
	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;

	gbState curState = gbState::Off;
	gbState nextState = curState;
	std::function<void(void)> clickFunc;
	float elapsedTime = 0;
	DXMath::Vector2 pos = { 150,150 };
	D2DRenderComponent* imagedata;
	BoxCollider* colliderdata;
	std::string_view imageFilepath;
	//1 , 2,   3            4 5 6        7  8  9
	//0+1 0+2 0+3           1*3+1  1*3+2 1*3+3      2*3 +1  2*3+ 2  2*3+3
	int index = 1;  
};

