#pragma once
#include "../Engine/Script.h"
#include "../Engine/DirectXInput.h"

class Object;
class D2DRenderComponent;
class ClickNextBimapScript : public Script , public IinputProcesser
{
public:
	ClickNextBimapScript(Object* _own) : Script(_own) {};
	virtual ~ClickNextBimapScript() = default;

	virtual void ComponentSetting() override;              // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();


private:
	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);

public:

private:
	D2DRenderComponent* ownerD2D;
	bool isClicked = false;
	int bitmapSize = 0;
	int index = 0;
};

