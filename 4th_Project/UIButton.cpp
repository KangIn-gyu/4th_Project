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
	//GetComponent<ButtonColider>()->SetBoundBox(0, 0, { imagedata->Get2DImageSize().x,imagedata->Get2DImageSize().y,0 });
	imagedata->Set2DImagePos(150, 150);
	CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { 150 + xy.x/2,150 + xy.y/2, 0.f};
	std::cout << imagedata->Get2DImageSize().x << " dawdawd" << imagedata->Get2DImageSize().y << std::endl;
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };
	GetComponent<BoxCollider>()->SetBox(center, extent, DXMath::Quaternion::Quaternion(0,0,0,1));
}

void UIButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
}

void UIButton::OnClick()
{
	std::cout << "2d´­·¶À½ " << std::endl;
	clickFunc();
}