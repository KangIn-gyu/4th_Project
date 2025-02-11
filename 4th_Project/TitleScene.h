#pragma once
#include "../Engine/Scene.h"

class D3DBaseObj;
class D3DAniObj;
class D2DBaseObj;
class TransformComponent;
class TitleScene : public Scene
{
public:
	TitleScene(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();
private:

public:

private:
	Object* mainCamera{};
	D3DAniObj* Evelyn{};
	D3DBaseObj* map{};
	D2DBaseObj* titleLogo{};
	D2DBaseObj* titleClick{};
	D2DBaseObj* Backgraund{};

	DXMath::Vector3 TargetPosition{};

	float startX{};
	bool movement = true;
	TransformComponent* cameraTransformComponent{};
};

