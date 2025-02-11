#pragma once
#include "../Engine/Scene.h"
#include "../Engine/DirectXInput.h"

class D2DBaseObj;

class DialogScene1 : public Scene
{
public:
	DialogScene1(std::string_view _Name) : Scene(_Name) {}

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

