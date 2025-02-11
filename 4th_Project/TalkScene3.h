#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class TalkScene3 : public Scene
{
public:
	TalkScene3(std::string_view _Name) : Scene(_Name) {}

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

