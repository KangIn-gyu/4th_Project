#pragma once
#include "../Engine/Object.h"

class BaseObj : public Object
{
public:
	BaseObj(std::string_view _name, Object::ObjectType _type);
	virtual void Initialize();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

private:


};

