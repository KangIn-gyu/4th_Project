#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"

Object::Object()
{
	CreateComponent<TransformComponent>();
}
