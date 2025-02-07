#include "pch.h"
#include "D2DBaseObj.h"
#include "../Engine/D2DRenderComponent.h"

void D2DBaseObj::Initialize()
{
	CreateComponent<D2DRenderComponent>();
}
