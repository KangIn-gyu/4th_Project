#pragma once
#include "../Engine/Object.h"

class AnimationObj : public Object
{
public:
	AnimationObj(std::string_view _name, Object::ObjectType _type);

	virtual void Initialize();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
};

