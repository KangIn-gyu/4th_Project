#pragma once
#include "Object.h"
#include "FactorySystem.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Initialize();
	void Update(float _deltaTime);
	void Render() {}; // TODO: 보류 사항 렌더를 여기서 전부 처리 해야 될까?

	void MainCameraSetting(int _index);
	void AddObject(Object* _obj);

	template <succession_Object T>
	T* GetGameObject(std::string name);

	void ShowObject(); 	// 확인용 
	const std::unordered_map<std::type_index, std::vector<Object*>> GetObjects() const { return Objects; }
private:
	void BasicObject();

public:

private:
	std::unordered_map<std::type_index, std::vector<Object*>> Objects{};

};

// 싱글톤으로 처리 안함
// 일단 보류

template<succession_Object T>
T* ObjectManager::GetGameObject(std::string name)
{
	auto it = Objects.find(typeid(T));
	if (it != Objects.end())
	{
		for (auto obj : it->second)
		{
			if (obj->GetName() == name)
			{
				return static_cast<T*>(obj);
			}
		}
	}
}
