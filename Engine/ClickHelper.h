#pragma once
#include "Object.h"

class Object;

DXMath::Vector2 ScreenToWorldRay(int mouseX, int mouseY, int screenWidth,
int screenHeight, const DXMath::Matrix& viewMatrix, const DXMath::Matrix& projectionMatrix, DXMath::Vector2& rayOrigin);
bool RayIntersectsAABB(
	DXMath::Vector2 rayOrigin,
	DXMath::Vector2 rayDirection,
	const DXMath::Vector2& boxMin,
	const DXMath::Vector2& boxMax);

class ClickHelper
{
public:
	static Object* FindObj(DXMath::Vector2 _rayOrigin, DXMath::Vector2 _rayDirection);
	static void checkClickobj(WPARAM _wParam, WPARAM _lParam);
	
};
