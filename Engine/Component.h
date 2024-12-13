#pragma once

class Object;
class Component // 추상 클래스 인터페이스라고 하기에는 함수 내용 적어서...
{
public:
	Component() {}
	virtual ~Component() {}

private:
	Object* owner {}; // 게임 오브젝트

public:
	const void SetComponentOwner(Object* _Owner) { owner = _Owner; }
	Object* ComponentOwner() { return owner; }

	virtual void ComponentInitialize() = 0;              // 초기화용
	virtual void ComponentUpdate(float _deltaTime) = 0;  // 업데이트
	virtual void ComponentRender() = 0;					 // 렌더용
};

template<class T>
concept ComponentType = std::is_base_of<Component, T>::value;
// 라이트랑 카메라도 컴포너트로 생각해야 한다.