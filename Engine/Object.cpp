#include "pch.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"  // ев╫╨©К 

Object::Object()
{
	CreateComponent<TransformComponent>();
}
