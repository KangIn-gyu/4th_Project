#pragma once

class Collider;
class IColliderNotify
{

public:
	virtual void OnBlock(Collider* _myCol, Collider* _otherCol) = 0; //stay등은 필요없을거같음
	virtual void EnterRayCollision(Collider* _otherCol) = 0;
	virtual void EndRayCollision(Collider* _otherCol) = 0;
	virtual ~IColliderNotify() = default;
};