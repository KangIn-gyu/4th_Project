#pragma once
#include "../Engine/Script.h"

class Object;
class D3DObjScript : public Script
{
public:
	D3DObjScript(Object* _own) : Script(_own) {};
	virtual ~D3DObjScript() = default;

	virtual void ComponentSetting() override;                       // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void ComponentUpdate(const float _deltaTime) override;  // 업데이트
};

