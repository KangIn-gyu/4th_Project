#include "pch.h"
#include "ClickFunc.h"
#include "Card.h"
#include "BlackJack.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "UIButton.h"
#include "MyGameManager.h"
#include "GambleButton.h"

void ClickFunc::SelectCard()
{
	
}
void ClickFunc::OpenButton()
{
	BLACKJACK->SetState(PlayerState::OPEN); //다른연출중일땐 클릭불가하게끔 hit은 카드받고 뒤집은다음, 스킬발동끝난다음
	/*Object* hitBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Hit");
	Object* stayBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Stay");
	Object* skillBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Skill");
	Object* openBtn = SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "Open");
	dynamic_cast<GambleButton*>(openBtn)->ChangeState(gbState::On);
	dynamic_cast<GambleButton*>(hitBtn)->ChangeState(gbState::Off);
	dynamic_cast<GambleButton*>(stayBtn)->ChangeState(gbState::Off);
	dynamic_cast<GambleButton*>(skillBtn)->ChangeState(gbState::Off);*/
}

void ClickFunc::StayButton()
{
	BLACKJACK->SetState(PlayerState::STAY);

}

void ClickFunc::HitButton()
{
	BLACKJACK->SetState(PlayerState::HIT);

}

void ClickFunc::SetPlayerSkill(Player* player, PSkill _skill)
{
	player->SetSkill(_skill);
	BLACKJACK->SetState(PlayerState::Skill);
	MYGAMEMANAGER->isBtnOn = false;

}

void ClickFunc::DoubleDown()
{
	//더블다운 컷신넣기 *****
	BLACKJACK->onDoubbleDown = true;
}

void ClickFunc::OnSetSkillBtn()
{
	if(MYGAMEMANAGER->isBtnOn == true)
		MYGAMEMANAGER->isBtnOn = false;
	else
		MYGAMEMANAGER->isBtnOn = true;
	
}




