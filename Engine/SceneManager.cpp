#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Helper.h"
#include "UserImGui.h"

void SceneManager::Update(const float _deltaTime)
{
	if(nullptr != currentScene)
	currentScene->Update(_deltaTime);
}

void SceneManager::FixedUpdate(const float _deltaTime)
{
	if (nullptr != currentScene)
	currentScene->FixedUpdate(_deltaTime);
}

void SceneManager::RateUpdate(const float _deltaTime)
{
	if (true == changeSceneTrigger)
	{
		changeSceneTrigger = false;
		std::invoke(&SceneManager::Change, SCENEMANAGER, changeSceneName);
	}

	if (nullptr != currentScene)
	currentScene->RateUpdate(_deltaTime);
}

void SceneManager::ChangeScene(std::string_view _SceneName)
{
	changeSceneTrigger = true;
	changeSceneName.assign(_SceneName);
	previousScene = currentScene;
	// TODO : 여기서 다음 씬에 필요한 오브젝트 처리하는걸 실행하면 좋을 거 같음
}

bool SceneManager::isCurrecntScene()
{
	return currentScene != nullptr;
}

void SceneManager::Change(std::string_view _SceneName)
{
	if (ScenesCollection.empty())
	{
		std::cout << "씬이 없습니다." << '\n';
	}

	auto it = ScenesCollection.find(_SceneName.data());
	if (it != ScenesCollection.end()) // 해당 씬이 있다면?
	{
		if(previousScene != nullptr)
			previousScene->SetActive(false);
		currentScene = it->second;
		if (currentScene != nullptr)
			currentScene->SetActive(true);

		currentScene->Initialize();
		currentScene->ResetInformation();
		currentScene->MainCameraSetting(0); // 메인 카메라 변경
		IMGUI->HierarchyCurrentSceneSetting(currentScene); 
	}
	else
	{
		std::cout << "입력하신 씬은 없습니다." << '\n';
	}
}

SceneManager::~SceneManager()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(ScenesCollection);
}




