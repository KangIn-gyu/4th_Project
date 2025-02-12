#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class Dealer;
class LobbyScene : public Scene
{
public:
	LobbyScene(std::string_view _Name);  


	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void ResetInformation();
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
private:

	
	float startX;  //플레이어 이동거리 제한인가
	float startZ;
	float endX;
	float endZ;
public:

private:
	Dealer* dealer{};
};

