#include "pch.h"
#include "ClickFunc.h"
#include "Card.h"
#include "BlackJack.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "UIButton.h"
void ClickFunc::SelectCard()
{
	
}
void ClickFunc::OpenButton()
{
	BLACKJACK->SetState(PlayerState::OPEN); //다른연출중일땐 클릭불가하게끔 hit은 카드받고 뒤집은다음, 스킬발동끝난다음
}

void ClickFunc::StayButton()
{
	BLACKJACK->SetState(PlayerState::STAY);
}

void ClickFunc::HitButton()
{
	BLACKJACK->SetState(PlayerState::HIT);
}

void ClickFunc::DoubleDown()
{
	//더블다운 컷신넣기 *****
	BLACKJACK->onDoubbleDown = true;
}

void ClickFunc::OnSetSkillBtn()
{
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill1")->SetActive(true);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill2")->SetActive(true);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill3")->SetActive(true);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill4")->SetActive(true);
}

void ClickFunc::SetPlayerSkill(Player* player, PSkill _skill)
{
	player->SetSkill(_skill);
	BLACKJACK->SetState(PlayerState::Skill);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill1")->SetActive(false);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill2")->SetActive(false);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill3")->SetActive(false);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill4")->SetActive(false);
}


