#pragma once
#include "SingletonBase.h"
#include "Object.h"

#define FACTORYSYSTEM FactorySystem::GetInstance()

template <typename T>
concept succession_Object = std::derived_from<T, Object>;

class CameraObject;
class FactorySystem : public SingletonBase<FactorySystem>
{
	friend class SingletonBase<FactorySystem>;
public:
	template<succession_Object T>
	T* CreateObject(std::string_view _objName , Object::ObjectType _type);

private:

public:

private:

};

template<succession_Object T>
T* FactorySystem::CreateObject(std::string_view _objName, Object::ObjectType _type)
{
	if (_type == Object::ObjectType::Basic && !std::is_same_v<T, CameraObject*>) // ! 카메라 오브젝트가 아니면 참
	{
		return new T(_objName, _type);
	}
	else if(_type == Object::ObjectType::Camera && std::is_same_v<T, CameraObject>)// 카메라 오브젝트가 맞으면 참
	{
		return new T(_objName, _type);
	}
	else
	{
		std::cout << "오브젝트 생성 실패" << std::endl;
		return nullptr;
	}
}

// registerType있는게 확장성으로 좋은데 소규모 프로젝트이기 때문에 간단하게 작성함