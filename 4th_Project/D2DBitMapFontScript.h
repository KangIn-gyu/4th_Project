#pragma once
#include "../Engine/Script.h"
// 용도 : 비트맵과 폰트를 같이 출력하기 위해 만듬
class D2DBitMapFontScript : public Script
{
public:
	D2DBitMapFontScript(Object* _own) : Script(_own) {};
	virtual ~D2DBitMapFontScript() = default;

	virtual void ComponentSetting() override;               // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation() {};
};

