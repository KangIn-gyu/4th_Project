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
	std::unordered_map<std::type_index, std::vector<Object*>> Objects;
};
// ΩÃ±€≈Ê¿∏∑Œ √≥∏Æ æ»«‘
