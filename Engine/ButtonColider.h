#pragma once
#include "Collider.h"
class AABB;

class ButtonColider : public Collider
{
public:
	ButtonColider() = default;
	~ButtonColider() = default;

	virtual void ComponentInitialize() override;                    // ??????
	virtual void ComponentUpdate(const float _deltaTime) override;  // ???????

	void SetBoundBox(int x, int y, D2D_VECTOR_3F bound);

	AABB* boundBox;
private:
	bool isActive = true;
};
