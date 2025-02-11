#pragma once
#include "Component.h"
class IColliderNotify;

enum class ActiveType
{
	None, // 단순 클릭검사등
	Block, //충돌시 못 넘어가게할것
};

enum class Touch
{
	None, //터치불가능
	Basic,
};
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
	 void OnRay(Collider* _otherCol);
	 void EndRay(Collider* _otherCol);
	virtual bool CheckCollision(Collider* _other) = 0;

	virtual void SetNotify(IColliderNotify* _notify) { notify = _notify; }
protected:

private:

public:
	ColliderType coltype;
	IColliderNotify* notify = nullptr;
	Collider* preCollision = nullptr; //이전충돌
	Collider* curCollision = nullptr; //지금
protected:

private:
};

