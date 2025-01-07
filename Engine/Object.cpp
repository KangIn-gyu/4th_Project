#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // 테스용 
#include "Helper.h"

Object::Object(Object::ObjectType _type) : type(_type)
{
	CreateComponent<TransformComponent>();
}

void Object::Update(const float _deltaTime)
{
    for (auto& map : components)
    {
        for (auto& component : map.second)
        {
            component->ComponentUpdate(_deltaTime);
        }
    }
}

void Object::ClearComponents()
{
    for (auto& pair : components)
    {
        for (Component* component : pair.second)
        {
            SafeExtinction::SAFE_DELETE(component); // 동적 할당된 메모리 해제
        } 
    }
    components.clear(); 
}
