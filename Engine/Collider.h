#pragma once
#include "Component.h"
class IColliderNotify;

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

	virtual	void OnBlock(Collider* _myCol, Collider* _otherCol);
	virtual bool CheckCollision(Collider* _other) = 0;
	virtual void SetNotify(IColliderNotify* _notify) { notify = _notify; }
protected:

private:

public:
	ColliderType type;
	IColliderNotify* notify = nullptr;
protected:

private:
};

