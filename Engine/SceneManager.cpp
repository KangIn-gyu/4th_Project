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

void SceneManager::AddGameObject(std::string_view _SceneName, Object* _ptr)
{
	auto it = ScenesCollection.find(_SceneName.data());

	if (it != ScenesCollection.end())
	{
		it->second->AddGameObject(_ptr->GetObjectType(), _ptr);
	}
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
		currentScene = it->second;

		if (previousScene != nullptr)
		{
			previousScene->SetActive(false);
		}
			
		if (currentScene != nullptr)
		{
			currentScene->SetActive(true);
		}

		currentScene->Initialize(); // 나중에 포토폴리오에 이 내용 꼭 써야됨 잘못되 초기화 방식 ㅠㅠ Initialize 이걸 유니티의 스타트처럼 사용하여 망함 진짜 반성하세요
		currentScene->ResetInformation();
		currentScene->MainCameraSetting(0); // 메인 카메라 변경

#ifdef IMGUIFLAG	
		IMGUI->HierarchyCurrentSceneSetting(currentScene); 
#endif
	}
	else
	{
		std::cout << "입력하신 씬은 없습니다." << '\n';
	}
}

Scene* SceneManager::GetScene(std::string_view _SceneName)
{
	auto it = ScenesCollection.find(_SceneName.data());

	if (it != ScenesCollection.end())
	{
		return it->second;
	}

	return nullptr;
}

std::vector<std::string> SceneManager::GetSceneNameList()
{ // 계속 생성 삭제를 해서 좀 그렇지만 걍 해 시간 없어
	std::vector<std::string> list;

	for(auto& it : ScenesCollection)
	{
		list.push_back(it.first);
	}

	return list;
}

SceneManager::~SceneManager()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(ScenesCollection);
}




