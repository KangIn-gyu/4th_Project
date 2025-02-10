#pragma once
#include "../Engine/Script.h"

class DialogButton;
class SelectionImageScript : public Script
{
public:
	SelectionImageScript(Object* _own) : Script(_own) {};
	virtual ~SelectionImageScript() = default;
	
	virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();
	void SetButton(DialogButton* _selectButton1, DialogButton* _selectButton2);

private:
	DialogButton* selectButton1{};
	DialogButton* selectButton2{};
};

