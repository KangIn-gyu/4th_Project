#include "pch.h"
#include "ToopTip2D.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/TransformComponent.h"

ToopTip2D::ToopTip2D(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos) : Object(_name, _type)
{
	imagedata = CreateComponent<D2DRenderComponent>();
	pos = _pos;
}

ToopTip2D::~ToopTip2D()
{

}

void ToopTip2D::Initialize()
{
	__super::Initialize();
	imagedata->Load2DImage("UI/ToolTip/" + GetName() + ".png"); 
	imagedata->Set2DImagePos(pos.x, pos.y);

}

void ToopTip2D::Update(const float _deltaTime)
{
}

