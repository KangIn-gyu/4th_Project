#include "pch.h"
#include "MyGameManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "UIButton.h"
#include "BlackJack.h"
#include "GambleButton.h"
void MyGameManager::Update(float _deltaTime)
{

	if (isBtnOn == true)
	{
		setSkillBtn(true);

		//if (true == CheckClick())
		//{
		//	isBtnOn = false;
		//	setSkillBtn(false);
		//}
	}
	else
	{
		setSkillBtn(false);
	}
	UpdateGambleButton();

	
}

void MyGameManager::UpdateGambleButton()
{

	PlayerState cur = BLACKJACK->GetState();
	Object* openBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Open");
	Object* hitBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Hit");
	Object* stayBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Stay");
	Object* skillBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Skill");

	// 모든 버튼을 기본적으로 Off로 설정

	if (pre != cur)
	{
		// 현재 상태에 맞는 버튼만 On으로 설정
		switch (cur)
		{
		case PlayerState::OPEN:
			dynamic_cast<GambleButton*>(openBtn)->ChangeState(gbState::On);
			dynamic_cast<GambleButton*>(hitBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(stayBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(skillBtn)->ChangeState(gbState::Off);
			break;
		case PlayerState::HIT:
			dynamic_cast<GambleButton*>(openBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(hitBtn)->ChangeState(gbState::On);
			dynamic_cast<GambleButton*>(stayBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(skillBtn)->ChangeState(gbState::Off);
			break;
		case PlayerState::STAY:
			dynamic_cast<GambleButton*>(openBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(hitBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(stayBtn)->ChangeState(gbState::On);
			dynamic_cast<GambleButton*>(skillBtn)->ChangeState(gbState::Off);
			break;
		case PlayerState::Skill:
			dynamic_cast<GambleButton*>(openBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(hitBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(stayBtn)->ChangeState(gbState::Off);
			dynamic_cast<GambleButton*>(skillBtn)->ChangeState(gbState::On);
			break;
		default:
			break;
		}
	}
	pre = cur;
}

void MyGameManager::setSkillBtn(bool _state)
{
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Handfaster")->SetActive(_state);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Guts")->SetActive(_state);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Meditation")->SetActive(_state);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Insurance")->SetActive(_state);
}

bool MyGameManager::CheckClick()
{
	const DX::Mouse::State& mouseteState = DXINPUT->mouse->GetState();
	return mouseteState.leftButton;
	

}
