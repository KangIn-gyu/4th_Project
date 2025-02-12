#include "pch.h"
#include "ClickChangeSceneScript.h"
#include "../Engine/Object.h"
#include "../Engine/SceneManager.h"
#include "D2DBaseObj.h"

void ClickChangeSceneScript::ComponentSetting()
{

}

void ClickChangeSceneScript::Update(const float _deltaTime)
{

}

void ClickChangeSceneScript::ResetInformation()
{
	ownerObject->SetActive(true);
}

void ClickChangeSceneScript::OnInputProcess(const DX::Keyboard::State& _KeyState, const DX::Keyboard::KeyboardStateTracker& _KeyTracker, const DX::Mouse::State& _MouseState, const DX::Mouse::ButtonStateTracker& _MouseTracker)
{
//	if (ownerObject->IsActive() == true)
//	{
//		if (_MouseTracker.rightButton == DX::Mouse::ButtonStateTracker::PRESSED || _MouseTracker.leftButton == DX::Mouse::ButtonStateTracker::PRESSED)
//		{
//			std::string changeSceneName = static_cast<D2DBaseObj*>(ownerObject)->sceneName;
//			if (!changeSceneName.empty())
//			{
//				SCENEMANAGER->ChangeScene(changeSceneName);
//			}
//		}
//	}
}
