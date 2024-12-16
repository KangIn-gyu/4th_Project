#pragma once
#include "Component.h"

// Tag 시스템은 나중에 생각하자 지금 하기에는 애매한 부분이 많다
class Object // 기반 클래스
{
public:
	Object();
	~Object() { ClearComponents(); }

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	template<ComponentType T>
	void CreateComponent();

	template<ComponentType T>
	T* GetComponent(int _index);
private:
	void ClearComponents();
public:

private:
	std::unordered_map<std::type_index, std::vector<Component*>> components;
};

template<ComponentType T>
inline void Object::CreateComponent()
{
	auto newComponent = new T;
	newComponent->SetComponentOwner(this);
	newComponent->ComponentInitialize();
	components[typeid(T)].emplace_back(std::move(newComponent));
}

// 타입을 넣고 해당 타입의 벡터를 받을 수 있다. 
// 장점으로 같은 타입의 컴포넌트가 있으면 그걸 인덱스로 접근해서 찾을 수 있다
template<ComponentType T>
inline  T* Object::GetComponent(int _index)
{
	auto it = components.find(typeid(T));
	if (it != components.end()) 
	{
		return dynamic_cast<T*>(it->second[_index]);
	}
//	return nullptr; // 나중에 로그 시스템으로 처리를 하자
}
