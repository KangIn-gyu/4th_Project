#pragma once
#include "../Engine/Scene.h"

class TestScene : public Scene
{
public:
	TestScene(std::string_view _Name);

private:
	virtual void Enter() override;
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};

public:

private:

};

