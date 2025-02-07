#pragma once
#include "../Engine/Scene.h"
#include "CardRotation.h"
class CardRotation;
class TestScene : public Scene
{
public:
	TestScene(std::string_view _Name);

private:
	virtual void Enter() override;
	virtual void Update(const float _deltaTime) override;
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	void Cards(DXMath::Vector3 pos);

public:
	CardRotation cardrot;
private:

};

