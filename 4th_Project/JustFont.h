#pragma once
#include "../Engine/Script.h"

class Object;
class D2DRenderComponent;


class JustFont : public Script
{
public:
	JustFont(Object* _own) : Script(_own) {};
	virtual ~JustFont() = default;

	void SetMessage(int* _value) { value = _value; } //출력할 메시지담기 베팅칩갯수 스킬포인트 등등
	virtual void ComponentSetting() override;              // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void Update(const float _deltaTime) override;  // 업데이트
	virtual void ResetInformation();

	int* value = nullptr;
private:
	D2DRenderComponent* ownerD2D;
	int bitmapSize = 0;
	int index = 0;
};

