#include "pch.h"
#include "ButtonColider.h"

#include "AABB.h"

void ButtonColider::SetBoundBox(int x, int y, D2D_VECTOR_3F bound)
{
	if (boundBox == nullptr)
		boundBox = new AABB();
	boundBox->SetExtent(bound.x / 2, bound.y / 2);
	boundBox->SetCenter(x, y);
}

