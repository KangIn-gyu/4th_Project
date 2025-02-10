#pragma once
#include "../Engine/Script.h"
#include "../Engine/DirectXInput.h"

class D2DRenderComponent;
class SelectionScript : public Script, public IinputProcesser
{
public:
	SelectionScript(Object* _own) : Script(_own) {};
	virtual ~SelectionScript() = default;

	virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();

private:
	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);

private:
	D2DRenderComponent* ownerD2D;
	std::wstring displayedText;
	std::vector<std::pair<int, std::wstring>> csvData;
	std::string changeSceneName;
	int index = 0;
	bool isClicked = false;

	int startImage = 0;
	int endImage = 0;
	bool isEnded = false;
};

