#include "pch.h"
#include "SceneManager.h"
#include "Helper.h"

SceneManager::~SceneManager()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(ScenesCollection);
	CurrentScene = nullptr;
}

void SceneManager::Initialize()
{

}

void SceneManager::LoadScene(Scene* _scene)
{
	auto it = ScenesCollection.find(_scene->GetName().data());
	if (it == ScenesCollection.end())
	{
		ScenesCollection.emplace(_scene->GetName().data(), std::move(_scene));
	}
}

