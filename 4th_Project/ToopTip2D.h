#pragma once
#include "../Engine/Object.h"

class D2DRenderComponent;
class ToopTip2D :public Object
{
public:
	ToopTip2D(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos);
	~ToopTip2D();


	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
private:
	D2DRenderComponent* imagedata;
	DXMath::Vector2 pos;
};

