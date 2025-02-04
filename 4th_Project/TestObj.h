#pragma once
#include "../Engine/Object.h"
#include "../Engine/IClick.h"
#include "../Engine/IOnmouse.h"

class TestObj : public Object, public IClick, public IOnmouse
{
public:
	TestObj(std::string_view _name , Object::ObjectType _type);

	virtual void Initialize();
	virtual void Update(const float _deltaTime) {};
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	virtual void OnClick() override;
	virtual void OnMouse() override;
};

