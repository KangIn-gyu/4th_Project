#pragma once
#include "Object.h"

// TOOD : 나중에 자동 폰트 오브젝트 생성하는걸 만들자 
class TextObject : public Object
{
public:
    TextObject(std::string_view _name, Object::ObjectType type = Object::ObjectType::UI);
    ~TextObject();

    virtual void Initialize(); 
    virtual void Update(const float _deltaTime) {};
    virtual void FixedUpdate() {}
    virtual void LateUpdate() {}

private:

public:

private:

};
