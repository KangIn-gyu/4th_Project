#pragma once
#include "../Engine/Scene.h"
class D2DBaseObj;
class DialogScene5 : public Scene
{
public:
	DialogScene5(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
private:
	D2DBaseObj* fading;
	D2DBaseObj* dialog;
public:

private:

};

