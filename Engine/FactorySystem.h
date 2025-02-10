#pragma once
#include "SingletonBase.h"
#include "Object.h"

#define FACTORYSYSTEM FactorySystem::GetInstance()

template <typename T>
concept succession_Object = std::derived_from<T, Object>;

template <typename T, typename... Args>
concept ObjectArgs = requires(Args&&... args)
{
	{ T(std::forward<Args>(args)...) } -> std::same_as<T>;  // 생성자가 올바르게 작동하는지 체크
};

class CameraObject;
class FactorySystem : public SingletonBase<FactorySystem>
{
	friend class SingletonBase<FactorySystem>;
public:
	template<succession_Object T, typename ... Arg>
	T* ObjectCreator(Arg&& ... _arguments);


private:

public:

private:

};

template<succession_Object T, typename ... Arg>
T* FactorySystem::ObjectCreator(Arg&& ... _arguments)
{
	if constexpr (ObjectArgs<T, Arg...>)
	{
		return new T(std::forward<Arg>(_arguments)...);
	}
	else
	{
		static_assert(ObjectArgs <T, Arg...>,
			"해당 오브젝트의 생성자 파라미터가 맞지 않습니다.");
	}
}
// registerType있는게 확장성으로 좋은데 소규모 프로젝트이기 때문에 간단하게 작성함
