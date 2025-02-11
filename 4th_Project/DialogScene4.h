#pragma once
#include "../Engine/Scene.h"
class D2DBaseObj;
class DialogScene4 : public Scene
{
public:
	DialogScene4(std::string_view _Name) : Scene(_Name) {}

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
private:
	D2DBaseObj* fading;
	D2DBaseObj* dialog;
public:

private:

};


