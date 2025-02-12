#pragma once
#include "../Engine/Script.h"
#include "../Engine/DirectXInput.h"

// 용도 : 비트맵과 폰트를 같이 출력하기 위해 만듬  급해서 다이얼로그 전용이 될거 같음
// CSV 로드도 포함한 다이얼로그 처리하는 곳이다.
class D2DRenderComponent;
class D2DBitMapFontScript : public Script , public IinputProcesser
{
public:
	D2DBitMapFontScript(Object* _own) : Script(_own) {};
	virtual ~D2DBitMapFontScript() = default;

	virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();
	void Reset();

private:
	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);

public:
	int index = 0;

private:
	D2DRenderComponent* ownerD2D;
	std::wstring displayedText;
	std::vector<std::pair<int, std::wstring>> csvData;
	std::string changeSceneName;

	bool isClicked = false;

	int startImage = 0;
	int endImage = 0;
};

