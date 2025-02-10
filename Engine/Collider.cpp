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
