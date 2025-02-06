#pragma once

class Object;
class Script
{
public:
	Script(Object* _ownerObject) : ownerObject(_ownerObject) {};
	virtual ~Script() {}

	virtual void ComponentSetting() = 0;                       // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void ComponentUpdate(const float _deltaTime) = 0;  // 업데이트

protected:
	Object* ownerObject; // 이걸 생성한 오브젝트가 주인이다.
};

