#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class Dealer;
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
	int success[2] = { 0, 2 };
	int failure[2] = { 3, 6 };

	bool isSet = false;
private:
	D2DBaseObj* q1{};
	D2DBaseObj* q2{};
	D2DBaseObj* q3{};
	D2DBaseObj* q4{};


	// 버튼 클릭시 패턴 랜덤으로 넣기 위해 처리

};
