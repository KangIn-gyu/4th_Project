#pragma once
#include "Scene.h"
class SceneManager
{
public:
	~SceneManager();

	void Initialize();
	void LoadScene(Scene* _scene);
private:

public:

private:
	std::unordered_map<std::string, Scene*> ScenesCollection;
	Scene* CurrentScene;
};

