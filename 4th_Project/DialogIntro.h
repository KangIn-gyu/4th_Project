#pragma once
#include "../Engine/Scene.h"

class DialogIntro : public Scene
{
public:
	DialogIntro(std::string_view _Name) : Scene(_Name) {}

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
private:

public:

private:

};
