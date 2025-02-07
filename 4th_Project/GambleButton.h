#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"
#include "../Engine/IOnmouse.h"

class D2DRenderComponent;

class GambleButton :public Object, public IClick
{
public:
	GambleButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func);
	GambleButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos, std::function<void()> _func);
	~GambleButton();


	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	virtual void OnClick() override;

	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;
private:
	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = { 150,150 };
};
