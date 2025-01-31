#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // ев╫╨©К 
#include "Helper.h"

Object::Object(std::string_view _name, ObjectType _type) : name(_name), type(_type)
{
	CreateComponent<TransformComponent>();
}

void Object::ComponentsUpdate(const float _deltaTime)
{
    for (auto& map : components)
    {
        for (auto& component : map.second)
        {
            component->ComponentUpdate(_deltaTime);
        }
    }
}

std::string Object::ObjectTypeToString()
{
    switch (type)
    {
    case ObjectType::Basic:
        return "Basic";
    case ObjectType::Light:
        return "Light";
    case ObjectType::Camera:
        return "Camera";
    case ObjectType::UI:
        return "UI";
    default:
        return "Unknown";
    }
}

bool Object::IsActive() const
{
    return state == State::Active;
}

bool Object::IsDead() const
{
    return  state == State::Dead;
}

void Object::SetActive(bool _state)
{
    if (_state == true)
    {
        state = State::Active;
    }
    else
    {
        state = State::Paused;
    }
}

void Object::Erase()
{
    state = State::Erase;
}

void Object::ClearComponents()
{
    SafeExtinction::SAFE_CLEAR_CONTAINER(components);
}

