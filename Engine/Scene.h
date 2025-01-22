#pragma once
#include "ObjectManager.h"
#include "Object.h"
#include "FactorySystem.h"


class Scene // 기반 클래스 이걸 상속해서 본인들이 원하는 씬을 만들면 됨
{
public:
	Scene(std::string_view _Name);
	virtual ~Scene();

	void Initialize();
	virtual void Enter() {};   // 해당 씬에서 내가 만들 오브젝트 초기화 하는 곳

	void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime) {};
	virtual void RateUpdate(const float _deltaTime) {};

	template<succession_Object T>
	void CreatorObject(std::string_view _name , Object::ObjectType _objType);

	template<succession_Object T, typename ... Arg>
	T* CreatorObject(Arg&& ... _arguments);
	
	void MainCameraSetting(const int _index);
	std::string GetName();

	void ShowObject();
	ObjectManager* GetObjectManager() { return objectManager; }
private:

public:

protected:
	std::string sceneName;
	ObjectManager* objectManager;
	bool is_initialize = false; 
private:

};

// 이걸 상속해서 씬을 만든 다음 그걸 로드 하면 된다.
// 기본 제공 오브젝트는 메인 카메라, 라이트를 제공.

template<succession_Object T>
void Scene::CreatorObject(std::string_view _name , Object::ObjectType _objType)
{
	objectManager->AddObject(FACTORYSYSTEM->CreateObject<T>(_name, _objType));
}


template<succession_Object T, typename ... Arg>
T* Scene::CreatorObject(Arg&& ... _arguments)
{
	auto object = FACTORYSYSTEM->CreateObject<T>(std::forward<Arg>(_arguments)...);
	objectManager->AddObject(object);
	return object; // 생성한 객체를 반환
}