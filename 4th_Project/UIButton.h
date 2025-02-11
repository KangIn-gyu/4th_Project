#pragma once
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/IClick.h"
#include "../Engine/Object.h"

class BoxCollider;
class D2DRenderComponent;

class UIButton :public Object, public IClick
{
public:
	UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func);
	UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos, std::function<void()> _func);
	UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos,
	         DXMath::Vector2 _size, std::function<void()> _func);
	~UIButton();


	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}

	void SetD2DLayerOrder(int _index);
	virtual void OnClick() override;

	void SetOnClick(std::function<void()> _func);

	D2DRenderComponent* imagedata;
	BoxCollider* colliderdata;
	std::string_view imageFilepath;

	bool onClick = false; // 클릭시 불과 전달

private:
	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = {150,150};
	DXMath::Vector2 size = { 0,0 };
};
