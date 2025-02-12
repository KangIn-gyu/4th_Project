#include "pch.h"
#include "GambleButton.h"

#include "../Engine/D2DRenderComponent.h"
#include "../Engine/BoxCollider.h"
#include "BlackJack.h"
#include "../Engine/TimeSystem.h"
#include "../Engine/SoundSystem.h"

GambleButton::GambleButton(std::string_view _name, Object::ObjectType _type,DXMath::Vector2 _pos, std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _name;
	imagedata = CreateComponent<D2DRenderComponent>();
}

GambleButton::~GambleButton()
{
}

void GambleButton::Initialize()
{
	Object::Initialize();
	imagedata->Load2DImage("UI/Button/" + GetName() + ".png"); //0
	imagedata->Load2DImage("UI/Button/" + GetName() + "_On.png");     //1
	imagedata->Load2DImage("UI/Button/" + GetName() + "_Toggle.png"); //2
	imagedata->Set2DImagePos(pos.x, pos.y);  // "UI/Button/" +Getname() + ".png" or + "_Click.png"
	colliderdata = CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x / 2,  pos.y + xy.y / 2, 0.f };
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };

	colliderdata->SetBox(center, extent, DXMath::Quaternion::Quaternion(0, 0, 0, 1));
	colliderdata->isDiamond = TRUE;
	D2D1_RECT_F box = colliderdata->GetBoundBox();
	imagedata->SetBoundBox(box);
	imagedata->IsDiamond(TRUE);
}

void GambleButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
	ChangeBit();
	//현재상태일떈 계속 점등하게 on -> basic -> onbasic

}

void GambleButton::Blink()
{
	float delta = TIMESYSTEM.get()->GetFloatDeltaTime();
	elapsedTime += delta;

	int n = static_cast<int>(fmod(elapsedTime, 1.0f) >= 0.5f); // 0.5초마다 0  1 전환
	imagedata->ChangeBitmap(n);
}

void GambleButton::ChangeBit()
{
	curState = nextState;
		switch (curState)
		{
		case gbState::On:
			Blink();
			break;
		case gbState::Toggle:
			imagedata->ChangeBitmap(2);
			break;
		default:
			imagedata->ChangeBitmap(0);
			break;
		}
}

void GambleButton::ChangeState(gbState _state)
{
	if (_state != nextState)
	{
		nextState = _state;
	}
}

void GambleButton::OnClick()
{
	//블랙잭의 상태에 따라조건 stay 누를수있는조건 skill 조건 ㅇ
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Button_Click, eSoundChannel::Effect);
	if(BLACKJACK->firstTurn == false && PLAYER->OnSkill == false)
		clickFunc();
}

void GambleButton::OnMouse()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Button_Hover, eSoundChannel::Effect);
	if(nextState != gbState::On || curState != gbState::On)
		ChangeState(gbState::Toggle);
}

void GambleButton::ExitMouse()
{
	if(nextState != gbState::On)
		ChangeState(gbState::Off);
}

