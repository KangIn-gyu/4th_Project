#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"
#include "../Engine/IOnmouse.h"

class D2DRenderComponent;

enum class dbState
{
	Off,
	Click,
	Toggle,
};

class DialogButton : public Object, public IClick, public IOnmouse
{
public:
	DialogButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func);
	DialogButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<bool()> _func);
	~DialogButton() {};

	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void ChangeBit();
	void ChangeState(dbState _state);//그림바꾸기용

	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;

	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;

	bool isClick = false;

private:
	dbState curState = dbState::Off;
	dbState nextState = curState;

	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = { 150,150 };

};

