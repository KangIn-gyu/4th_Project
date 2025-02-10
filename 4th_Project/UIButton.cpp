#include "pch.h"
#include "UIButton.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/BoxCollider.h"

UIButton::UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, std::function<void()> _func) :Object(_name, _type)
{
	clickFunc = _func;
	imageFilepath = _filePath;
	imagedata = CreateComponent<D2DRenderComponent>();
}

UIButton::UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos, std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _filePath;
	imagedata = CreateComponent<D2DRenderComponent>();
}
UIButton::UIButton(std::string_view _name, Object::ObjectType _type, std::string_view _filePath, DXMath::Vector2 _pos, DXMath::Vector2 _size, std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _filePath;
	imagedata = CreateComponent<D2DRenderComponent>();
	size.x = _size.x;
	size.y = _size.y;
}
UIButton::~UIButton()
{
}

void UIButton::Initialize()
{
	Object::Initialize();

	imagedata->Load2DImage(imageFilepath);
	imagedata->Set2DImagePos(pos.x, pos.y);
	if (size.x!=0)
		imagedata->Set2DImageSize(size.x, size.y);

	colliderdata = CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x/2,  pos.y + xy.y/2,    0.f};
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };

	colliderdata->SetBox(center, extent, DXMath::Quaternion::Quaternion(0,0,0,1));
	D2D1_RECT_F box = colliderdata->GetBoundBox();
	//imagedata->SetBoundBox(box);
}

void UIButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
}

void UIButton::SetD2DLayerOrder(int _index)
{
	imagedata->bitmapLayerOrder = _index;
}

void UIButton::OnClick()
{
	std::cout << "2d´­·¶À½ " << std::endl;
	clickFunc();
}