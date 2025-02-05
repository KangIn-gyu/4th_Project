#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // 테스용 
#include "Helper.h"

Object::Object(std::string_view _name, ObjectType _type) : name(_name), type(_type)
{
	CreateComponent<TransformComponent>();
}

Object::~Object()
{
    ClearComponents();
    SafeExtinction::SAFE_DELETE(script);
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

    if (nullptr != script)
    {
        script->ComponentUpdate(_deltaTime);
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

    if (isActive != _state)
    {
        for (auto& component : components)
        {
            if (component.second.empty()) continue; // 비어 있는 경우 방어 코드 추가

            for (auto com : component.second)
            {
                if (com == nullptr) continue; // nullptr 체크
                com->SetActive(_state);
            }
        }
    }

}

void Object::Erase()
{
    state = State::Erase;
    //setactive 끄고 위치도 10000,10000
    //Layer Erase 모아뒀다가 내가 특정 타입을 부르면 >> erase 있으면? 얘를 갖다줘 없으면 그냥 생성
}

void Object::ComponentSetting()
{
    script->ComponentSetting();
}

void Object::ClearComponents()
{
    SafeExtinction::SAFE_CLEAR_CONTAINER(components);
}

