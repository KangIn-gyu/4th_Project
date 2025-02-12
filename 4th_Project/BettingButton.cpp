#include "pch.h"
#include "BettingButton.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/BoxCollider.h"
#include "BlackJack.h"
#include "../Engine/TimeSystem.h"
#include "../Engine/SoundSystem.h"

BettingButton::BettingButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _name;
	imagedata = CreateComponent<D2DRenderComponent>();
}

BettingButton::~BettingButton()
{
}

void BettingButton::Initialize()
{
	Object::Initialize();
	imagedata->Load2DImage("UI/Button/Fold.png");        //0  3   6   i index 0 = fold
	imagedata->Load2DImage("UI/Button/Fold_Toggle.png");     //1  4   7  index 1 = Raise
	imagedata->Load2DImage("UI/Button/Fold_Toggle.png"); //2  5   8    index 2 = All in
	imagedata->Load2DImage("UI/Button/Raise.png");
	imagedata->Load2DImage("UI/Button/Raise_Toggle.png");
	imagedata->Load2DImage("UI/Button/Raise_Toggle.png");
	imagedata->Load2DImage("UI/Button/All In.png");       
	imagedata->Load2DImage("UI/Button/All In_Toggle.png");  
	imagedata->Load2DImage("UI/Button/All In_Toggle.png");
	imagedata->Set2DImagePos(pos.x, pos.y);  
	colliderdata = CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x / 2,  pos.y + xy.y / 2, 0.f };
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };
	colliderdata->SetBox(center, extent, DXMath::Quaternion::Quaternion(0, 0, 0, 1));
	D2D1_RECT_F box = colliderdata->GetBoundBox();
	imagedata->SetBoundBox(box);
}

void BettingButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);

	if (BLACKJACK->endBet != true)
	{
		ChangeState(gbState::On);
	}
	else
	{
		ChangeState(gbState::Off);
	}
	ChangeBit();
}

void BettingButton::Blink()
{
		float delta = TIMESYSTEM.get()->GetFloatDeltaTime();
		elapsedTime += delta;
		int n = static_cast<int>(fmod(elapsedTime, 1.0f) >= 0.5f); // 0.5초마다 0  1 전환
		imagedata->ChangeBitmap(index * 3 + n);
}

void BettingButton::ChangeBit()
{

	//index + 1;  //기본; //잠시빤짝임; //온 마우스
	curState = nextState;
	switch (curState)
	{
	case gbState::On:
		Blink(); //0 + 1        3+1    6+1   1   4  7
		break;
	case gbState::Toggle:
		imagedata->ChangeBitmap(index * 3 + 2);
		break;
	default:
		imagedata->ChangeBitmap(index * 3 + 0);
		break;
	}
}

void BettingButton::ChangeState(gbState _state)
{
	if (_state != nextState)
	{
		nextState = _state;
	}
}

void BettingButton::OnClick()
{
	if (BLACKJACK->firstTurn == false)
	{
		BLACKJACK->Bet();
		if (BLACKJACK->betType == BetType::Fold)
		{
			SOUNDSYSTEM->PlayMusic(eSoundList::VS_Fold, eSoundChannel::Voice);
		}
		else if (BLACKJACK->betType == BetType::Raise)
		{
			SOUNDSYSTEM->PlayMusic(eSoundList::VS_Raise, eSoundChannel::Voice);
		}
		else
		{
			SOUNDSYSTEM->PlayMusic(eSoundList::SE_All_In, eSoundChannel::Voice);
		}
	}
}

void BettingButton::OnMouse()
{
	if (nextState != gbState::On || curState != gbState::On) //off가 기본 on은 순간 깜빡임
		ChangeState(gbState::Toggle);
}

void BettingButton::ExitMouse()
{
	if (nextState != gbState::On)
		ChangeState(gbState::Off);
}
