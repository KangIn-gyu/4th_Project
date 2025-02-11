#pragma once
#include "../Engine/Scene.h"
#include "../Engine/DirectXInput.h"

class UIButton;
class D2DBaseObj;

class DialogScene1 : public Scene
{
public:
	DialogScene1(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
private:
	D2DBaseObj* fading;
	D2DBaseObj* dialog;
	UIButton* skipbutton{};
public:

private:

};

