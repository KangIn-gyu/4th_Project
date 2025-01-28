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

	template <typename T>
	T* GetGameObject(Object::ObjectType _type,std::string _name);
	void ShowObject(); 	// 확인용 
	const std::unordered_map<Object::ObjectType, std::vector<Object*>> GetObjects() const;

private:
	void BasicObject();

public:

private:
	std::unordered_map<Object::ObjectType, std::vector<Object*>> Objects{};
};

template<typename T>
T* ObjectManager::GetGameObject(Object::ObjectType _type, std::string _name)
{
	auto it = Objects.find(_type);
	if (it != Objects.end())
	{
		for (auto& gameobj : it->second)
		{
			if (gameobj->GetName() == _name)
			{
				return static_cast<T*>(gameobj);
			}			
		}
	}
	return nullptr; 
}

// 싱글톤으로 처리 안함
// 일단 보류
