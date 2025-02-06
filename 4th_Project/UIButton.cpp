#include "pch.h"
#include "UIButton.h"

#include "../Engine/D2DRenderComponent.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/ButtonColider.h"
#include "../Engine/BoxCollider.h"

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

	CreateComponent<TransformComponent>()->SetPosition({ 50 ,50,0 });
	imagedata->Set2DImagePos(50, 50);

	CreateComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetBox({ imagedata->Get2DImageSize().x / 2, imagedata->Get2DImageSize().y / 2 ,0.1 }, { imagedata->Get2DImageSize().x/2, imagedata->Get2DImageSize().y/2 ,0 }, DXMath::Quaternion::Quaternion(0,0,0,1));
}

void UIButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
}

void UIButton::OnClick()
{
	std::cout << "2ddddd" << std::endl;
	clickFunc();
}