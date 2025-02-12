#pragma once
#include "SingletonBase.h"
#include "Scene.h"

#define SCENEMANAGER SceneManager::GetInstance()

template<class T>
concept SceneType = std::derived_from<T, Scene>;

class SceneManager : public SingletonBase<SceneManager>
{
	friend class SingletonBase<SceneManager>;
public:
	template<SceneType T>
	void CreatorScene(std::string_view _SceneName);

	void Update(const float _deltaTime);
	void FixedUpdate(const float _deltaTime);
	void RateUpdate(const float _deltaTime);

	void ChangeScene(std::string_view _SceneName);
	void AddGameObject(std::string_view _SceneName, Object* _ptr);

	bool isCurrecntScene();
	Scene* GetCurrentScene() const { return currentScene; }

	Scene* GetScene(std::string_view _SceneName);

	std::vector<std::string> GetSceneNameList();
private:
	virtual ~SceneManager();
	void Change(std::string_view _SceneName);

public:
	bool isTalking = false;
private:
	Scene* currentScene{}; // ÇöÀç ¾À
	std::unordered_map<std::string, Scene*> ScenesCollection; // ¾À ÀúÀåÇÏ´Â °÷
	//Scene* currentScene {}; // ÇöÀç ¾À
	bool changeSceneTrigger = false;
	Scene* previousScene{};
	std::string changeSceneName;
};

template<SceneType T>
void SceneManager::CreatorScene(std::string_view _SceneName)
{
	T* scene = new T(_SceneName);
	ScenesCollection[_SceneName.data()] = scene;
}
