#include "pch.h"
#include "JustBox.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/TransformComponent.h"
JustBox::JustBox(std::string_view _name, ObjectType _type, DXMath::Vector3 _pos,DXMath::Vector3 _extent, DXMath::Vector3 _center) : Object(_name, _type)
{
	CreateComponent<BoxCollider>();
	DXMath::Vector3 extent = _extent;
	DXMath::Vector3 center = _center;
	GetComponent<TransformComponent>()->SetPosition(_pos);
	GetComponent<BoxCollider>()->SetBox(center, extent, {0,0,0,1}, ActiveType::Block);
	GetComponent<BoxCollider>()->coltouch = Touch::None;
}

void JustBox::Initialize()
{
}

void JustBox::Update(const float _deltaTime)
{
}
