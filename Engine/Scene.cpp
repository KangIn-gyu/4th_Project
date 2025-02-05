#include "pch.h"
#include "Scene.h"

#include "Helper.h"
#include "Object.h"
#include "SceneManager.h"
#include "EventSystem.h"

#include "CameraObject.h"
#include "TransformComponent.h"

#include "DOTweenManager.h"
Scene::Scene(std::string_view _Name)
{
	sceneName.assign(_Name);
	SCENEMANAGER->LoadScene(this);
	CreateLayers();
	BasicObject();
}

Scene::~Scene()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(gameObecjts);
}

void Scene::Initialize()
{
	Enter(); // 생성된 오브젝트 오브젝트 매니저에 넣기
	if (is_initialize == false)
	{
		for (auto& obj : gameObecjts)
		{
			obj->Initialize();

		}

		is_initialize = true;
	}
}

void Scene::Update(const float _deltaTime)
{
	for (auto& obj : gameObecjts)
	{
		obj->Update(_deltaTime);
	}

	eventSysyem->Update();
	dotweenManager->Update(_deltaTime);
}

void Scene::MainCameraSetting(const int _index)
{
	if (_index >= 0 && _index < gameObecjts[static_cast<int>(Layer::Tag::Camera)]->GetSize())
	{
		CameraObject::g_MainCameraObject = static_cast<CameraObject*>(gameObecjts[static_cast<int>(Layer::Tag::Camera)]->GetGameObject(_index));
	}
	else
	{ // 추후 로그 시스템으로 처리해야 됨
		std::cout << "카메라 세팅 실패" << '\n';
	}
}

const std::string& Scene::GetName()
{
	return sceneName;
}

Object* Scene::GetGameObject(Object::ObjectType _Type, std::string_view _name)
{
	return gameObecjts[static_cast<int>(_Type)]->GetGameObject(_name);
}

Object* Scene::GetGameObject(Object::ObjectType _Type, int _index)
{
	return gameObecjts[static_cast<int>(_Type)]->GetGameObject(_index);
}

std::vector<Layer*> Scene::NextSceneUseObjcet()
{
	for (auto& layer : gameObecjts)
	{ // TODO : 씬 이동후 다음 씬에 이동이 필요한 오브젝트 처리 해야됨
	//	layer->GetGameObject
	}

	return nextSceneUseObjects;
}

void Scene::CreateLayers()
{
	gameObecjts.resize(static_cast<int>(Layer::Tag::End));

	for (int i = 0; i < static_cast<int>(Layer::Tag::End); ++i)
	{
		// Tag 값으로 Layer 객체 생성
		Layer* newLayer = new Layer(static_cast<Layer::Tag>(i));
		gameObecjts[i] = newLayer;  // gameObecjts에 추가
	}
}

void Scene::BasicObject()
{
	auto* mainCamera = FACTORYSYSTEM->ObjectCreator<CameraObject>("MainCamera", Object::ObjectType::Camera);
	mainCamera->GetComponent<TransformComponent>()->SetPosition({ 0, 0, -300.0f });
	gameObecjts[static_cast<int>(Object::ObjectType::Camera)]->AddGameObjcet(mainCamera);

	// TODO : 라이트 추가 해야됨.
}
