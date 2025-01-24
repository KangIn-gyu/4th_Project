#include "pch.h"
#include "EventSystem.h"
#include "Object.h"
#include "IClick.h"
#include "IOnmouse.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraObject.h"
#include "BoxCollider.h"
#include <DirectXTK/Mouse.h>
#include "DirectXInput.h"
#include "Engine.h"

EventSystem::EventSystem()
{
}

Object* EventSystem::FindObj(DXMath::Vector3 _rayOrigin, DXMath::Vector3 _rayDirection)
{
	float closestDistance = FLT_MAX;  // 가장 가까운 거리 (초기값은 매우 큰 값)
	Object* closestObject = nullptr; // 가장 가까운 오브젝트 포인터

	for (const auto& [type, objs] : SCENEMANAGER.get()->currentScene->GetObjectManager()->GetObjects()) {
		// 각 오브젝트들에 대해 순차적으로 검사 일단 Basic타입이랑 UI타입만하게했는대
		// 나중에 클릭할 오브젝트만 따로담아두는게
		if (Object::ObjectType::Basic == type || Object::ObjectType::UI == type)
		{
			for (const auto& obj : objs) {
				// 레이가 AABB와 교차하는지 확인)
				auto boxcol = obj->GetComponent<BoxCollider>();
				if (boxcol != nullptr)
				{
					float distance;
					if (boxcol->IntersectsRay(_rayOrigin, _rayDirection, distance))
					{
						if (distance < closestDistance) {
							closestDistance = distance;  // 가장 가까운 거리 갱신
							closestObject = obj;  // 가장 가까운 오브젝트 저장
						}
					}
				}
			}
		}
		else
		{
			continue;
		}
	}

	return closestObject;
}

void EventSystem::checkClickobj(int _mouseX, int _mouseY)
{

	//std::cout << " 클릭 감지됨" << std::endl;


	//일단 상수 나중에 받아오기
	int screenWidth = Engine::GetInstance().get()->GetWindowSize().x;
	int screenHeight = Engine::GetInstance().get()->GetWindowSize().y;

	std::cout << _mouseX << " " << _mouseY << std::endl;
	DXMath::Ray ray = GenerateRayFromMouse(_mouseX, _mouseY, screenWidth, screenHeight, CameraObject::g_MainCameraObject->GetViewMatrix()
		, CameraObject::g_MainCameraObject->GetProjectionMatrix());

	Object* curobj = FindObj(ray.position, ray.direction);
	if (curobj != nullptr)
	{
		//std::cout << curobj->name << std::endl;
	}
	IClick* ClickAble = dynamic_cast<IClick*>(curobj);
	if (ClickAble)
		ClickAble->OnClick();
}

void EventSystem::Update()
{
	OnmouseEvent();
}

void EventSystem::OnmouseEvent()
{

	int mouseX = DXINPUT.get()->mouseState.x;
	int mouseY = DXINPUT.get()->mouseState.y;

	int screenWidth = Engine::GetInstance().get()->GetWindowSize().x;
	int screenHeight = Engine::GetInstance().get()->GetWindowSize().y;

	DXMath::Ray ray = GenerateRayFromMouse(mouseX, mouseY, screenWidth, screenHeight, CameraObject::g_MainCameraObject->GetViewMatrix()
		, CameraObject::g_MainCameraObject->GetProjectionMatrix());
	Object* curobj = FindObj(ray.position, ray.direction);
	if (curobj != nullptr)
	{
		//std::cout << curobj->name << std::endl;
	}
	IOnmouse* Onmouse = dynamic_cast<IOnmouse*>(curobj);
	if (Onmouse)
		Onmouse->OnMouse();
}



DXMath::Ray GenerateRayFromMouse(int mouseX, int mouseY, int screenWidth, int screenHeight, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix)
{
	using namespace DirectX;

	// 스크린 좌표를 정규화 디바이스 좌표(NDC)로 변환
	float ndcX = (2.0f * mouseX / screenWidth) - 1.0f;
	float ndcY = 1.0f - (2.0f * mouseY / screenHeight); // Y는 상하 반전

	// 역행렬 계산
	XMMATRIX inverseVP = XMMatrixInverse(nullptr, viewMatrix * projectionMatrix);

	// NDC 좌표를 월드 공간으로 변환
	XMVECTOR nearPoint = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 0.0f, 1.0f), inverseVP);
	XMVECTOR farPoint = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 1.0f, 1.0f), inverseVP);

	// 레이 방향 계산 (정규화)
	XMVECTOR rayDirection = XMVector3Normalize(farPoint - nearPoint);

	// 결과 반환
	return DXMath::Ray{
	   DXMath::Vector3(nearPoint.m128_f32[0], nearPoint.m128_f32[1], nearPoint.m128_f32[2]),
	   DXMath::Vector3(rayDirection.m128_f32[0], rayDirection.m128_f32[1], rayDirection.m128_f32[2])
	};
}