#pragma once
#include "Object.h"

class TestObj : public Object
{
public:
	virtual void Start();
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
};

