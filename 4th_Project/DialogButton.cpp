#include "pch.h"
#include "DialogButton.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/BoxCollider.h"
#include "BlackJack.h"

DialogButton::DialogButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func) : Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _name;
	imagedata = CreateComponent<D2DRenderComponent>();
}

void DialogButton::Initialize()
{
	Object::Initialize();
	imagedata->Load2DImage("UI/Button/" + GetName() + ".png"); //0
	imagedata->Load2DImage("UI/Button/" + GetName() + "_Click.png");     //1
	imagedata->Load2DImage("UI/Button/" + GetName() + "_Toggle.png"); //2
	imagedata->Set2DImagePos(pos.x, pos.y);
	CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x / 2,  pos.y + xy.y / 2, 0.f };
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };
	GetComponent<BoxCollider>()->SetBox(center, extent, DXMath::Quaternion::Quaternion(0, 0, 0, 1));
}

void DialogButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
	ChangeBit();
}

void DialogButton::ChangeBit()
{
	curState = nextState;
	switch (curState)
	{
	case dbState::Click:
		imagedata->ChangeBitmap(1);
		break;
	case dbState::Toggle:
		imagedata->ChangeBitmap(2);
		break;
	default:
		imagedata->ChangeBitmap(0);
		break;
	}
}

void DialogButton::ChangeState(dbState _state)
{
	if (_state != nextState)
	{
		nextState = _state;
	}
}

void DialogButton::OnClick()
{
	isClick = true;
	clickFunc();
}

void DialogButton::OnMouse()
{
	if (nextState != dbState::Click || curState != dbState::Click)
		ChangeState(dbState::Toggle);
}

void DialogButton::ExitMouse()
{
	if (nextState != dbState::Click)
		ChangeState(dbState::Off);
}
