#include "pch.h"
#include "UIButton.h"

#include "../Engine/D2DRenderComponent.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/ButtonColider.h"

UIButton::UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func) :Object(_name, _type)
{
	clickFunc = _func;
	imageFilepath = _filePath;
}

UIButton::~UIButton()
{
}

void UIButton::Initialize()
{
	Object::Initialize();
	imagedata = CreateComponent<D2DRenderComponent>();
	imagedata->Load2DImage(imageFilepath);
	GetComponent<ButtonColider>()->SetBoundBox(0, 0, { imagedata->Get2DImageSize().x,imagedata->Get2DImageSize().y,0 });
}

void UIButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
}

void UIButton::OnClick()
{
	clickFunc();
}
