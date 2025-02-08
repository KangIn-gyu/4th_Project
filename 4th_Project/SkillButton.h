#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"
#include "../Engine/IOnmouse.h"
#include "GambleButton.h"
class D2DRenderComponent;


class SkillButton :public Object, public IClick, public IOnmouse
{
public:

	SkillButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, int _cost,std::function<void()> _func);
	~SkillButton();


	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void ChangeBit();
	void ChangeState(gbState _state);//그림바꾸기용
	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;
	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;

private:
	int cost = 5; //각 스킬 코스트
	gbState curState = gbState::Off;
	gbState nextState = curState;
	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = { 150,150 };
	float elapsedTime = 0;
};
