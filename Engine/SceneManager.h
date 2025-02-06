#pragma once

#include "SingletonBase.h"

#define SCENEMANAGER SceneManager::GetInstance()
class Scene;
class SceneManager : public SingletonBase<SceneManager>
{
	friend class SingletonBase<SceneManager>;
public:
	void Update(const float _deltaTime);
	void FixedUpdate(const float _deltaTime);
	void RateUpdate(const float _deltaTime);

	void ChangeScene(std::string_view _SceneName);

	bool isCurrecntScene();
	Scene* GetCurrentScene() const { return currentScene; }

	void LoadScene(Scene* _scene);  // 씬이 생성되면 자동으로 함
private:
	virtual ~SceneManager();
	void Change(std::string_view _SceneName);

public:
	
private:
	Scene* currentScene{}; // 현재 씬
	std::unordered_map<std::string, Scene*> ScenesCollection; // 씬 저장하는 곳
	//Scene* currentScene {}; // 현재 씬
	bool changeSceneTrigger = false;
	Scene* previousScene{};
	std::string changeSceneName;
};
// 순수하게 씬만 관리
