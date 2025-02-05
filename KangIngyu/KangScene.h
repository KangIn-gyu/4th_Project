#pragma once
#include "../Engine/Scene.h"

class KangScene : public Scene
{
public:
	KangScene(std::string_view _Name);

private:
	virtual void Enter() override;
	virtual void Initialize() override;
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};

public:

private:
};

