#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // 테스용 

Object::Object()
{
	CreateComponent<TransformComponent>();
    CreateComponent<ModelComponent>();
}

void Object::ClearComponents()
{
    for (auto& pair : components)
    {
        for (Component* component : pair.second)
        {
            delete component; // 동적 할당된 메모리 해제
        }
        pair.second.clear(); 
    }
    components.clear(); 
}
