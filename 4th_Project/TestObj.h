#pragma once
#include "../Engine/Object.h"

class TestObj : public Object
{
public:
	TestObj(Object::ObjectType _type);

	virtual void Start();
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
};

