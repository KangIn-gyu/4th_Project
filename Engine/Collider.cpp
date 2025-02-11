#include "pch.h"
#include "Collider.h"
#include "IColliderNotify.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::OnBlock(Collider* _myCol, Collider* _otherCol)
{
	if (notify != nullptr)
		notify->OnBlock(_myCol, _otherCol);
}

void Collider::OnRay(Collider* _otherCol)
{
	if (notify != nullptr)
		notify->EnterRayCollision(_otherCol);
}

void Collider::EndRay(Collider* _otherCol)
{
	if (notify != nullptr)
		notify->EndRayCollision(_otherCol);
}

