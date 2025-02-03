#pragma once
#include "Object.h"

// TOOD : 나중에 자동 폰트 오브젝트 생성하는걸 만들자 
class TextObject : public Object
{
public:
    TextObject(std::string_view _name, Object::ObjectType type = Object::ObjectType::UI);
    ~TextObject();

    virtual void Initialize() {};  // 용도 : 내가 필요한 컴포넌트 생성하는 곳 초기화나
    virtual void Update(const float _deltaTime) {}; // 용도 : 오브젝트 개인의 업데이트가 필요할때 정의
    virtual void FixedUpdate() {}
    virtual void LateUpdate() {}

private:

public:

private:

};
