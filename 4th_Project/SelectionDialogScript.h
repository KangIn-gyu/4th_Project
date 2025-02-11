#pragma once
#include "../Engine/Script.h"
#include "../Engine/DirectXInput.h"

// D2DBitMapFontScript와 기능은 비슷함 단지 모든 대화를 출력하고선 버튼 2개를 보여줌
class D2DRenderComponent;
class UIButton;
class DialogButton;
class SelectionDialogScript : public Script, public IinputProcesser
{
public:
	SelectionDialogScript(Object* _own) : Script(_own) {};
	virtual ~SelectionDialogScript() = default;

	virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();

	// 넣어진 버튼은 상태를 꺼놓음
	void SetButton(DialogButton* _selectButton1, DialogButton* _selectButton2);
	//void SetButton(DialogButton* _selectButton1, DialogButton* _selectButton2, DialogButton* _);
	void ButtonChangeIndex(int _index);
private:
	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker);

public:

private:
	D2DRenderComponent* ownerD2D;
	std::wstring displayedText;
	std::vector<std::pair<int, std::wstring>> csvData;
	int index = 0;
	int startImage = 0;
	int endImage = 0;

	bool isClicked = false;
	DialogButton* selectButton1{};
	DialogButton* selectButton2{};
};

