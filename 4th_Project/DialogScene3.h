#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class DialogScene3 : public Scene
{

public:
	DialogScene3(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation() override;
private:

public:

private:
	D2DBaseObj* dialog{};
};
