#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class GambleScene : public Scene
{
public:
	GambleScene(std::string_view _Name);

private:
	virtual void Enter() override;
	virtual void Update(const float _deltaTime) override;
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();

public:

private:
	D2DBaseObj* q1{};
	D2DBaseObj* q2{};
	D2DBaseObj* q3{};
	D2DBaseObj* q4{};
};
