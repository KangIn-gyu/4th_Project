#pragma once
#include "Component.h"

// Tag 시스템은 나중에 생각하자 지금 하기에는 애매한 부분이 많다
class TransformComponent;
class Object // 기반 클래스
{
public:
	Object();
	~Object() {};

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	template<ComponentType T>
	void CreateComponent();

private:

public:

private:
	std::vector<Component*> components;
};

template<ComponentType T>
inline void Object::CreateComponent()
{
	T* NewComponent = new T();  
	NewComponent->SetComponentOwner(this);
	components.emplace_back(NewComponent);
}
