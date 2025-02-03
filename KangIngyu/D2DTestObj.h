#pragma once
#include "../Engine/Object.h"

class D2DTestObj : public Object
{
public:
	D2DTestObj(std::string_view _name, Object::ObjectType type = Object::ObjectType::UI) : Object(_name, type) {};
	~D2DTestObj() = default;

	virtual void Initialize() override;  // 용도 : 내가 필요한 컴포넌트 생성하는 곳 초기화나
	virtual void Update(const float _deltaTime) {}; // 용도 : 오브젝트 개인의 업데이트가 필요할때 정의
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

private:
	
};

