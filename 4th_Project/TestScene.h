#pragma once
#include "../Engine/Scene.h"
#include "CardRotation.h"
#include "../Engine/DirectXInput.h"
class CardRotation;
class TestScene : public Scene, public IinputProcesser
{
public:
	TestScene(std::string_view _Name) : Scene(_Name) {};

private:
	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();

public:
	CardRotation cardrot;

private:
	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);
};

