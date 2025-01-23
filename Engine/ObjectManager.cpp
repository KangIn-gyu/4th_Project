#include "pch.h"
#include "ObjectManager.h"
#include "CameraObject.h"
#include "Helper.h"
#include "FactorySystem.h"
#include "TransformComponent.h"

void ObjectManager::MainCameraSetting(int _index)
{
	if (_index >= 0 && _index < Objects[Object::ObjectType::Camera].size())
	{
		CameraObject::g_MainCameraObject = static_cast<CameraObject*>(Objects[Object::ObjectType::Camera][_index]);
	}
	else
	{ // 추후 로그 시스템으로 처리해야 됨
		std::cout << "카메라 세팅 실패" << '\n';
	}
}

void ObjectManager::AddObject(Object* _obj)
{
	if (_obj)
	{
		// 객체의 타입에 해당하는 vector에 추가
		Objects[_obj->GetObjectType()].push_back(_obj);
		//std::cout << typeid(*_obj).name() << " ADD" << '\n';
	}
}

void ObjectManager::ShowObject()
{
	for (auto& it : Objects)
	{
		// std::cout << it.first << '\n'; // TODO :  이넘클래스 형변환 체크 필요
		for (auto& vecData : it.second)
		{
			std::cout << " ShowObject : " << typeid(*vecData).name() << "  Type : " << vecData->ObjectTypeToString() << '\n';
		}
	}
}

const std::unordered_map<Object::ObjectType, std::vector<Object*>> ObjectManager::GetObjects() const
{
	return Objects;
}

/// <summary>
/// 유니티처럼 기본적으로 제공하는 오브젝트 예로 카메라, 라이트 이렇게만 할 예정
/// </summary>
void ObjectManager::BasicObject()
{ // TODO: 현재 라이트가 없음 추가 필요

	auto* mainCamera = FACTORYSYSTEM->CreateObject<CameraObject>("MainCamera", Object::ObjectType::Camera);
	mainCamera->GetComponent<TransformComponent>()->SetPosition({ 0, 0, -300.0f });
	Objects[Object::ObjectType::Camera].push_back(mainCamera);
}

ObjectManager::ObjectManager()
{
	BasicObject(); // 생성과 동시에 기본 오브젝트 생성
}

ObjectManager::~ObjectManager()
{
	SafeExtinction::SAFE_CLEAR_CONTAINER(Objects);
}

void ObjectManager::Initialize()
{
	for (auto& obj : Objects)
	{
		for (int i = 0; i < obj.second.size(); i++)
		{
			obj.second[i]->Start();
		}
	}
}

void ObjectManager::Update(float _deltaTime)
{
	for (auto& obj : Objects)
	{
		for (int i = 0; i < obj.second.size(); i++)
		{
			obj.second[i]->ComponentsUpdate(_deltaTime); // 오브젝트는 컴포넌트를 먼저 업데이트를 하고
			obj.second[i]->Update(_deltaTime);           // 개인이 업데이트 해야 될거 처리
		}
	}
}

