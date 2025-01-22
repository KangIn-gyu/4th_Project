#include "pch.h"
#include "Scene.h"

#include "Helper.h"
#include "Object.h"
#include "SceneManager.h"
#include "EventSystem.h"
Scene::Scene(std::string_view _Name)
{
	sceneName.assign(_Name);
	objectManager = new ObjectManager;
	SCENEMANAGER->LoadScene(this);
}

Scene::~Scene()
{
	std::cout << "¾À »èÁ¦" << "\n";
	SafeExtinction::SAFE_DELETE(objectManager);
}

void Scene::Initialize()
{
	if (is_initialize == false)
	{
		objectManager->Initialize();
		is_initialize = true;
	}
}

void Scene::Update(const float _deltaTime)
{
	objectManager->Update(_deltaTime);
	eventSysyem->Update();
}

void Scene::MainCameraSetting(const int _index)
{
	objectManager->MainCameraSetting(_index);
}

std::string Scene::GetName()
{
	return sceneName;
}

void Scene::ShowObject()
{
	objectManager->ShowObject();
}
