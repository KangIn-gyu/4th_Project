#pragma once
#include "../Engine/Scene.h"
class UIButton;
class D2DBaseObj;

class CreditScene : public Scene
{
public:
	CreditScene(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
private:
	D2DBaseObj* fading;
	D2DBaseObj* loopImg;

public:

private:

};
