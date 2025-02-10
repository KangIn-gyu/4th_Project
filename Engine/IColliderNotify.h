#pragma once

class Collider;
class IColliderNotify
{

public:
	virtual void OnBlock(Collider* _myCol, Collider* _otherCol) = 0; //stay등은 필요없을거같음
	virtual ~IColliderNotify() = default;
};