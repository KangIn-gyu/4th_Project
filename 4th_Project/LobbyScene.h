#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class LobbyScene : public Scene
{
public:
	LobbyScene(std::string_view _Name) : Scene(_Name) {}

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
private:

	float startX;
	float startZ;
	float endX;
	float endZ;
public:

private:

};

