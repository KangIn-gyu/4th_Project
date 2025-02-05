#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"

class D2DRenderComponent;

class UIButton :public Object, public IClick
{
public:
	UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func);
	~UIButton();

	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	virtual void OnClick() override;

	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;
private:
	std::function<void(void)> clickFunc;
};
