#include "pch.h"
#include "ObjectManager.h"
#include "Object.h"
#include "CameraObject.h"
#include "Helper.h"


#include "TestObj.h"

void ObjectManager::TestCode() 
{ // 테스트 용도 오브젝트 생성
	CameraObject* mainCamera = new CameraObject;
	CameraObject::g_MainCameraObject = mainCamera; // 메인 카메라 설정 
 	Objects[typeid(CameraObject)].push_back(mainCamera);

	TestObj* testObj = new TestObj();
	Objects[typeid(Object)].push_back(testObj);
}

void ObjectManager::MainCameraSetting(int _index)
{
	if (_index > 0 && _index < Objects[typeid(CameraObject)].size())
	{
		CameraObject::g_MainCameraObject = static_cast<CameraObject*>(Objects[typeid(CameraObject)][_index]);
	}
	else
	{ // 추후 로그 시스템으로 처리해야 됨
		std::cout << "카메라 세팅 실패" << '\n';
	}
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

