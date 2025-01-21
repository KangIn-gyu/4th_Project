#pragma once
#include "Object.h"

class Object;


DXMath::Ray GenerateRayFromMouse(int mouseX, int mouseY, int screenWidth, int screenHeight,
	const DirectX::XMMATRIX& viewMatrix,
	const DirectX::XMMATRIX& projectionMatrix);

class ClickHelper
{
public:
	static Object* FindObj(DXMath::Vector3 _rayOrigin, DXMath::Vector3 _rayDirection);
	static void checkClickobj(int _mouseX, int mouseY);
	
};
