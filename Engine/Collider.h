#pragma once
#include "Component.h"
enum class ColliderType
{
	Box,
	Circle, //일단2개
};
// public 함수 -> protected 함수 -> private 함수 -> public 변수 -> protected 변수 -> private 변수 }
class Collider : public Component
{
public:
	Collider();
	~Collider();

protected:

private:

public:
	ColliderType type;

protected:

private:
};

