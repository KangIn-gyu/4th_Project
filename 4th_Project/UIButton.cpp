#include "pch.h"
#include "UIButton.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/BoxCollider.h"

UIButton::UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func) :Object(_name, _type)
{
	clickFunc = _func;
	imageFilepath = _filePath;
}

UIButton::UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos, std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
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
	imagedata->Set2DImagePos(pos.x, pos.y);
	CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x/2,  pos.y + xy.y/2,    0.f};
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };
	GetComponent<BoxCollider>()->SetBox(center, extent, DXMath::Quaternion::Quaternion(0,0,0,1));
}

void UIButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
}

void UIButton::OnClick()
{
	onClick = true;
	std::cout << "2d´­·¶À½ " << std::endl;
	clickFunc();
}