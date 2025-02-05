#pragma once
#include "Object.h"
#include "FactorySystem.h"
#include "Layer.h"

class Object;
class Scene // 기반 클래스 이걸 상속해서 본인들이 원하는 씬을 만들면 됨
{
public:
	Scene(std::string_view _Name);
	virtual ~Scene();

	void Initialize();
	virtual void Enter() {}; // 해당 씬에서 내가 만들 오브젝트 초기화 하는 곳 Initialize 보다 먼저 시작됨으로 오브젝트를 생성하고 이후 오브젝트의 초기화를 한다

	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate(const float _deltaTime) {};
	virtual void RateUpdate(const float _deltaTime) {};

	template<succession_Object T, typename ... Arg>
	T* ObjectCreator(std::string_view _name , Object::ObjectType _Type, Arg&& ... _arguments);

	void MainCameraSetting(const int _index = 0);
	const std::string& GetName();
	void EraseGameObject(Layer::Tag tag, Object* obj);
	void SetActive(bool _Active);
	std::vector<Layer*>& GetGameObecjts() { return gameObecjts; } // ImGui 오브젝트 데이터 넘기기용 사용하지 마시오

	Object* GetGameObject(Object::ObjectType _Type, std::string_view _name);
	Object* GetGameObject(Object::ObjectType _Type, int _index = 0);

	std::vector<Layer*> NextSceneUseObjcet();
private:
	void CreateLayers();
	void BasicObject(); // 기본 제공 오브젝트

public:

protected:
	std::string sceneName;
	bool is_initialize = false; 
	std::vector<Layer*> gameObecjts;
	std::vector<Layer*> nextSceneUseObjects;

private:

};

// 이걸 상속해서 씬을 만든 다음 그걸 로드 하면 된다.
// 기본 제공 오브젝트는 메인 카메라, 라이트를 제공.

template<succession_Object T, typename ... Arg>
T* Scene::ObjectCreator(std::string_view _name , Object::ObjectType _Type, Arg&& ... _arguments)
{
	if constexpr (sizeof...(_arguments) == 0)
	{
		auto object = FACTORYSYSTEM->ObjectCreator<T>(_name, _Type);
		gameObecjts[static_cast<int>(_Type)]->AddGameObjcet(object);
		return object; // 생성한 객체를 반환
	}
	else
	{
		auto object = FACTORYSYSTEM->ObjectCreator<T>(_name, _Type, std::forward<Arg>(_arguments)...);
		gameObecjts[static_cast<int>(_Type)]->AddGameObjcet(object);
		return object; // 생성한 객체를 반환
	}
}
