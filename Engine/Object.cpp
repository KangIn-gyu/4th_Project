#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // 테스용 
#include "Helper.h"

Object::Object()
{
	CreateComponent<TransformComponent>();
}

void Object::ClearComponents()
{
    for (auto& pair : components)
    {
        for (Component* component : pair.second)
        {
            SafeExtinction::SAFE_DELETE(component); // 동적 할당된 메모리 해제
        }
        pair.second.clear(); 
    }
    components.clear(); 
}
