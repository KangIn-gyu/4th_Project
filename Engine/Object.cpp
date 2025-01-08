#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // ев╫╨©К 
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
    SafeExtinction::SAFE_CLEAR_CONTAINER(components);
}
