#pragma once
#include "../Engine/IClick.h"
#include "../Engine/Object.h"

class D2DRenderComponent;

class UIButton :public Object, public IClick
{
public:
	UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func);
	UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos, std::function<void()> _func);
	~UIButton();


	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	
	virtual void OnClick() override;

	D2DRenderComponent* imagedata;
	std::string_view imageFilepath;

	// 버튼이 클릭되었을때 다이얼로그한테 상태 전달용
	bool onClick = false; 

private:
	std::function<void(void)> clickFunc;
	DXMath::Vector2 pos = {150,150};

};
