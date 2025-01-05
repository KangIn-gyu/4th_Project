#pragma once
#include "Component.h"


class Material;
class RenderComponent : public Component
{
public:
	RenderComponent();
	virtual ~RenderComponent() {};

	virtual void ComponentInitialize() override;
	virtual void ComponentUpdate(const float _deltaTime) override {};
	virtual void ComponentRender()override;

private:

public:

private:
	std::shared_ptr<Material> material; // ∞ÌπŒ¡ﬂ

};
