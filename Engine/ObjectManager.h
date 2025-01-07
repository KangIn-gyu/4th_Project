#pragma once

class Object;

class ObjectManager
{
public:
	~ObjectManager();

	void Initialize();
	void Updata(float _deltaTime);
	void Render() {}; // 보류 사항

	void TestCode();
	void MainCameraSetting(int _index);
private:

public:

private:
	std::unordered_map<std::type_index, std::vector<Object*>> Objects {};
};

// 싱글톤으로 처리 안함
// 일단 보류
