#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"
#include "../Engine/IOnmouse.h"

class D2DRenderComponent;


enum class gbState
{
	Off,
	On,
	Toggle,
};
class GambleButton :public Object, public IClick, public IOnmouse
{
public:

	GambleButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func);
	~GambleButton();


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
	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;
	
private:
	gbState curState = gbState::Off;
	gbState nextState = curState;
	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = { 150,150 };
	float elapsedTime = 0;
};
