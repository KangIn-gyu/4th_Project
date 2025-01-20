#pragma once
#include "../Engine/Object.h"

class TestObj2 : public Object
{
public:
	TestObj2(std::string_view _name, Object::ObjectType _type);

	virtual void Start();
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
};

