#pragma once

class Object;
class ObjectManager
{
public:
	void Updata(float _deltaTime);
	void Render();

private:

public:

private:
	std::unordered_map<std::type_index, std::vector<Object*>> Objects {};
};
// 싱글톤으로 처리 안함
// 일단 보류
