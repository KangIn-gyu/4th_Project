#include "pch.h"
#include "UIToggleBtn.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/BoxCollider.h"
#include "BlackJack.h"
#include "../Engine/TimeSystem.h"
#include "../Engine/SoundSystem.h"

UIToggleBtn::UIToggleBtn(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _name;
	imagedata = CreateComponent<D2DRenderComponent>();
}

UIToggleBtn::~UIToggleBtn()
{
}

void UIToggleBtn::Initialize()
{
	Object::Initialize();
	imagedata->Load2DImage("UI/Button/" + GetName() + ".png"); //0
	//imagedata->Load2DImage("UI/Button/" + GetName() + "_On.png");     //1
	imagedata->Load2DImage("UI/Button/" + GetName() + "_Toggle.png"); //1
	imagedata->Set2DImagePos(pos.x, pos.y);  // "UI/Button/" +Getname() + ".png" or + "_Toggle.png"
	colliderdata = CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x / 2,  pos.y + xy.y / 2, 0.f };
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };
	colliderdata->SetBox(center, extent, DXMath::Quaternion::Quaternion(0, 0, 0, 1));
	D2D1_RECT_F box = colliderdata->GetBoundBox();
	imagedata->SetBoundBox(box);
}

void UIToggleBtn::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);

}


void UIToggleBtn::SetOnClick(std::function<void()> _func)
{
	clickFunc = _func;
}

void UIToggleBtn::SetD2DLayerOrder(int _index)
{

	imagedata->bitmapLayerOrder = _index;

}

void UIToggleBtn::OnClick()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Button_Click, eSoundChannel::Effect);
	clickFunc();
}

void UIToggleBtn::OnMouse()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Button_Hover, eSoundChannel::Effect);
	imagedata->ChangeBitmap(1); //마우스올리면 1로
}

void UIToggleBtn::ExitMouse()
{
	imagedata->ChangeBitmap(0); //마우스올리면 1로
}
