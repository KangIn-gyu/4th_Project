#pragma once
#include "../Engine/Scene.h"

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

};
