#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "CameraObject.h"
#include "Helper.h"

void ObjectManager::TestCode() 
{ // 테스트 용도 오브젝트 생성
	CameraObject* mainCamera = new CameraObject;
	Objects[typeid(CameraObject)].push_back(mainCamera);
}

ObjectManager::~ObjectManager()
{
	for (auto& obj : Objects)
	{
		for (auto& index : obj.second)
		{
			SafeExtinction::SAFE_DELETE(index);
		}
	}
}

void ObjectManager::Initialize()
{
	TestCode();

	for (auto& obj : Objects)
	{
		for (int i = 0; i < obj.second.size(); i++)
		{
			obj.second[i]->Start();
		}
	}
}

void ObjectManager::Updata(float _deltaTime)
{
	for (auto& obj : Objects)
	{
		for (int i = 0; i < obj.second.size(); i++)
		{
			obj.second[i]->Update(_deltaTime);
		}
	}
}

