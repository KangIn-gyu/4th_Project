#pragma once
#include "../Engine/Object.h"
class D2DRenderComponent;

class hogamdo :public Object
{
public:
	hogamdo(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos);
	~hogamdo();

	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;
	DXMath::Vector2 pos = { 150,150 };
};

