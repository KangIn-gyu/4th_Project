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

	// 확인용 
	void ShowObject();
private:
	void BasicObject();

public:

private:
	std::unordered_map<std::type_index, std::vector<Object*>> Objects{};

};

// 싱글톤으로 처리 안함
// 일단 보류

// 