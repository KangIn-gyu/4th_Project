#pragma once
#include "../Engine/Scene.h"
#include "../Engine/DirectXInput.h" // 테스트용으로 만듬

class D2DBaseObj;
class UIButton;
class TutorialScene : public Scene , public IinputProcesser
{
public:
	TutorialScene(std::string_view _Name);

	virtual void Enter() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime)override {};
	virtual void RateUpdate(const float _deltaTime)override {};
	virtual void ResetInformation();

private:
	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);

public:

private:
	UIButton* skipbutton{};
	D2DBaseObj* tutorialBitmap{};
};

