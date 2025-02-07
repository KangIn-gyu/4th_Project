#include "pch.h"
#include "MyGameManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "UIButton.h"

MyGameManager::~MyGameManager()
{
}

void MyGameManager::Update(float _deltaTime)
{

	if (isBtnOn == true)
	{
		setSkillBtn(true);

		if (true == CheckClick())
		{
			isBtnOn = false;
			setSkillBtn(false);
		}
	}
	
}

void MyGameManager::setSkillBtn(bool _state)
{
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill1")->SetActive(_state);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill2")->SetActive(_state);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill3")->SetActive(_state);
	SCENEMANAGER.get()->GetCurrentScene()->GetGameObject(Object::ObjectType::UI, "SKill4")->SetActive(_state);
}

bool MyGameManager::CheckClick()
{
	const DX::Mouse::State& mouseteState = DXINPUT->mouse->GetState();
	return mouseteState.leftButton;
	

}
