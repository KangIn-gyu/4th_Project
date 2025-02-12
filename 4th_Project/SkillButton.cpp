#include "pch.h"
#include "SkillButton.h"
#include "../Engine/D2DRenderComponent.h"
#include "../Engine/TransformComponent.h"
#include "../Engine/BoxCollider.h"
#include "BlackJack.h"
#include "../Engine/TimeSystem.h"
#include "../Engine/SceneManager.h"
#include "../Engine/SoundSystem.h"

SkillButton::SkillButton(std::string_view _name, Object::ObjectType _type, DXMath::Vector2 _pos, int _cost,std::function<void()> _func) :Object(_name, _type)
{
	pos = _pos;
	clickFunc = _func;
	imageFilepath = _name;
	imagedata = CreateComponent<D2DRenderComponent>();

	cost = _cost;
	
}

SkillButton::~SkillButton()
{
}

void SkillButton::Initialize()
{
	Object::Initialize();
	imagedata->Load2DImage("UI/Button/" + GetName() + ".png"); //0 기본
	imagedata->Load2DImage("UI/Button/" + GetName() + "_On.png");     //1  //꺼진상태 or 블링크
	imagedata->Load2DImage("UI/Button/" + GetName() + "_Toggle.png"); //2  // 마우스올린상태
	imagedata->Set2DImagePos(pos.x, pos.y);  // "UI/Button/" +Getname() + ".png" or + "_Toggle.png"
	colliderdata = CreateComponent<BoxCollider>();
	auto xy = imagedata->Get2DImageXY();
	DXMath::Vector3 center = { pos.x + xy.x / 2,  pos.y + xy.y / 2, 0.f };
	DXMath::Vector3 extent = { xy.x / 2 ,xy.y / 2 , 0.1f };
	colliderdata->SetBox(center, extent, DXMath::Quaternion::Quaternion(0, 0, 0, 1));
	D2D1_RECT_F box = colliderdata->GetBoundBox();
	imagedata->SetBoundBox(box);
}

void SkillButton::Update(const float _deltaTime)
{
	Object::Update(_deltaTime);
	if (PLAYER->skillPoint >= cost && PLAYER->canSkill == true) //
		ChangeState(gbState::On);
	else
		ChangeState(gbState::Off);
	ChangeBit();


}


void SkillButton::ChangeBit()
{
	curState = nextState;
	switch (curState)
	{
	case gbState::On:
		imagedata->ChangeBitmap(0);
		break;
	case gbState::Off:
		imagedata->ChangeBitmap(1);
		break;
	case gbState::Toggle:
		imagedata->ChangeBitmap(2);
		break;
	default:
		imagedata->ChangeBitmap(0);
		break;
	}
}

void SkillButton::ChangeState(gbState _state)
{
	if (_state != nextState)
		nextState = _state;
}

void SkillButton::OnClick()
{
	SOUNDSYSTEM->PlayMusic(eSoundList::SE_Button_Click, eSoundChannel::Effect);
	if (curState == gbState::On && PLAYER->OnSkill == false)
	{
		PLAYER->skillPoint -= cost;
		PLAYER->OnSkill = true;
		clickFunc();
	}
}

void SkillButton::OnMouse()
{
	if (isOn == true)
	{
		SOUNDSYSTEM->PlayMusic(eSoundList::SE_Button_Hover, eSoundChannel::Effect);
		isOn = false;
	}

	if (nextState != gbState::On || curState != gbState::On) // //툴팁출력*****
		ChangeState(gbState::Toggle);

	//툴팁찾아서 출력
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, GetName() + "_ToolTip")->SetActive(true);
}

void SkillButton::ExitMouse()
{
	isOn = true;
	if (nextState != gbState::On)                       
		ChangeState(gbState::Off);

	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, GetName() + "_ToolTip")->SetActive(false);
}

