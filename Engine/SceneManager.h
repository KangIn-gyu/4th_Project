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

	void LoadScene(Scene* _scene);  // 일단 테스트 씬 하나 제공하고선 처리해야 할듯
	void ChangeScene(std::string_view _SceneName);

private:
	virtual ~SceneManager();
public:

private:
	std::unordered_map<std::string, Scene*> ScenesCollection; // 씬 저장하는 곳
	Scene* currentScene {}; // 현재 씬

};
// 순수하게 씬만 관리
