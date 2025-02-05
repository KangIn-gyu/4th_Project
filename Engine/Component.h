#pragma once

class Object;
class Component // 추상 클래스 인터페이스라고 하기에는 함수 내용 적어서...
{
public:
	Component() {}
	virtual ~Component() {}

protected:
	Object* owner{}; // 게임 오브젝트
	bool    isActive = true;
private:

public:
	const void SetOwner(Object* _Owner) { owner = _Owner; }
	Object* GetOwner() { return owner; }
	
	virtual bool GetActive() { return isActive; }
	virtual void SetActive(bool _isActive);
	virtual void ComponentInitialize() = 0;                    // 초기화용
	virtual void ComponentUpdate(const float _deltaTime) = 0;  // 업데이트
//	virtual void ComponentRender() = 0;					      // TODO: 보류 사항 컴포넌트가 렌더러 업데이트를 해야 될가?
};

// 컨셉 정리
template<class T>
concept ComponentType = std::derived_from<T, Component>;
