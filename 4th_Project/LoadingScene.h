#pragma once
#include "../Engine/Scene.h"

class D2DBaseObj;
class LoadingScene : public Scene
{
public:
	LoadingScene(std::string_view _Name) : Scene(_Name) {}
	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
	void NextScene(std::string_view _Name) { nextSceneName = _Name; }
private:

public:

private:
	std::string nextSceneName{};
	float LoadingTime = 0;
	float maxLoadingTime = 0;
	D2DBaseObj* loadingImage;
};

