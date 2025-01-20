#pragma once
#include "Object.h"

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
	T* GetObjectss(std::string name)
	{
		auto it  = Objects.find(typeid(T));
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

// 