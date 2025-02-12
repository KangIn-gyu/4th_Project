#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"
#include "../Engine/IOnmouse.h"
#include "GambleButton.h"
class BoxCollider;
class D2DRenderComponent;

class UIToggleBtn :public Object, public IClick, public IOnmouse
{
public:
	UIToggleBtn(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func);
	~UIToggleBtn();

	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void SetD2DLayerOrder(int _index);


	virtual void OnClick() override;
	virtual void OnMouse() override;
	virtual void ExitMouse() override;
	D2DRenderComponent* imagedata;
	BoxCollider* colliderdata;
	std::string_view imageFilepath;
	//gbState curState = gbState::Off;
	//gbState nextState = curState;
	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = { 150,150 };
	float elapsedTime = 0;

	bool isOn = false;

};

