#pragma once
#include "Component.h"
#include <concepts>
#include <unordered_map>

// Tag 시스템은 나중에 생각하자 지금 하기에는 애매한 부분이 많다
// 컴포넌트는 생성과 동시에 컴포넌트의 초기화를 실행한다
class Object // 기반 클래스
{
public:
	enum class State
	{
		Active,  // 실행
		Paused,  // 일시 정지
		Dead,    // 죽음
		Erase,   // 삭제
		Next,    // 다음씬에 넘김
		End
	};

	enum class ObjectType // 이거 수정하면 Layer 태그도 수정해야됨.
	{
		Background,
		Camera,
		Light,
		Basic,
		UI,
		End
	};

	Object(std::string_view _name , Object::ObjectType _type = ObjectType::Basic); // 명시 안해놓으면 기본 오브젝트로 생성
	virtual ~Object() { ClearComponents(); }

	void ComponentsUpdate(const float _deltaTime);

	virtual void Initialize() {};  // 용도 : 내가 필요한 컴포넌트 생성하는 곳 초기화나
	virtual void Update(const float _deltaTime) {}; // 용도 : 오브젝트 개인의 업데이트가 필요할때 정의
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	State GetState() { return state; }
	ObjectType GetObjectType() { return type; }
	std::string ObjectTypeToString();
	
	std::string GetName() { return name; }

	void SetState(State _state) { state = _state; } // 오브젝트의 상태 변경

	template<ComponentType T>
	T* GetComponent(int _index = 0); // 기본 인덱스는 0으로 함

	bool IsActive() const;
	bool IsDead() const;

	void SetActive(bool _state);
	void Erase();

	int GetLayerOrder() { return layerOrder; };

	bool operator<(const Object& other) const
	{
		// 예시로 layerOrder를 기준으로 정렬
		return this->layerOrder < other.layerOrder;
	}
protected:
	template<ComponentType T, typename ... Arg> // 함수 오버로드함
	T* CreateComponent(Arg&&... _arguments);

private:
	void ClearComponents();

public:

protected:
	int layerOrder{}; // 2D일때 그리는 순서 정하게 할 경우

private:
	std::string name;
	State state = State::Active;  // 해당 타입은 set 만들면 안됨.
	ObjectType type;
	std::unordered_map<std::type_index, std::vector<Component*>> components;
};

template <typename T, typename... Args>
concept ComponentConstructibleWithArgs = requires(Args&&... args) 
{
	{ T(std::forward<Args>(args)...)} -> std::same_as<T>;  // 생성자가 올바르게 작동하는지 체크
};

template<ComponentType T, typename ... Arg>
T* Object::CreateComponent(Arg&& ... _arguments)
{
	if constexpr (sizeof...(_arguments) == 0)
	{
		auto* newComponent = new T;
		newComponent->SetOwner(this);
		newComponent->ComponentInitialize();
		if(nullptr != newComponent) // 이렇게 처리한 이유는 카메라 컴포넌트가 오브젝트 타입이 카메라가 아니면 오너를 삭제시킨다
		components[typeid(T)].emplace_back(std::move(newComponent));
		return newComponent;
	}
	else
	{
		if constexpr (ComponentConstructibleWithArgs <T, Arg...>)
		{
			auto* newComponent = new T(_arguments...);
			newComponent->SetOwner(this);
			newComponent->ComponentInitialize();
			if (nullptr != newComponent)
			components[typeid(T)].emplace_back(std::move(newComponent));
			return newComponent;
		}
		else
		{ 
			static_assert(ComponentConstructibleWithArgs <T, Arg...>,
				"해당 컴포넌트는 생성자의 파라미터가 맞지 않습니다.");
		}
	}
}

// 타입을 넣고 해당 타입의 벡터를 받을 수 있다. 
// 장점으로 같은 타입의 컴포넌트가 있으면 그걸 인덱스로 접근해서 찾을 수 있다
template<ComponentType T>
inline T* Object::GetComponent(int _index)
{
	auto it = components.find(typeid(T));
	if (it != components.end()) 
	{	
		// dynamic_cast을 하면 안전성을 더욱 보장하지만 어차피 템플릿을 통해서 
		// 확실한 타입을 알 수 있기 때문에 static_cast으로 처리함 그럼 더욱 성능 최적화가 된다.
		if (_index >= 0 && _index < components.size())
		{
			return static_cast<T*>(it->second[_index]);
		}
		else
		{ // 추후 로그 시스템으로 처리해야됨
			std::cout << "GetComponent 인덱스가 맞지 않습니다" << '\n';
		}
	}
	return nullptr; // 나중에 로그 시스템으로 처리를 하자
}

// dynamic_cast
// 런타임에 타입의 안정성을 검사하는 동적 캐스팅

// static_cast
// 컴파일 타임에 수행하는 정적 캐스팅을 수행