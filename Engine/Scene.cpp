#include "pch.h"
#include "Scene.h"

#include "Helper.h"

Scene::Scene(std::string_view _Name)
{
	sceneName.assign(_Name);
	objectManager = new ObjectManager;
}

Scene::~Scene()
{
	SafeExtinction::SAFE_DELETE(objectManager);
}

void Scene::Initialize()
{
	Enter();
	objectManager->Initialize();
}

void Scene::Update(const float _deltaTime)
{
	objectManager->Update(_deltaTime);
}

void Scene::FixedUpdate(const float _deltaTime)
{
	// 보류
}

void Scene::RateUpdate(const float _deltaTime)
{
	// 보류
}

std::string_view Scene::GetName()
{
	return sceneName;
}
