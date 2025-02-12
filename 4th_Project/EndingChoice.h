#pragma once
#include "../Engine/Scene.h"
class DialogButton;
class UIButton;
class D2DBaseObj;

class EndingChoice : public Scene
{
public:
	EndingChoice(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
private:
	D2DBaseObj* fading;
	D2DBaseObj* dialog;
	DialogButton* happyButton;
	DialogButton* badButton;
public:

private:

};
