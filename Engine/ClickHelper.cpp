#include "pch.h"
#include "ClickHelper.h"
#include "Object.h"
#include "IClick.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraObject.h"
Object* ClickHelper::FindObj(DXMath::Vector2 _rayOrigin, DXMath::Vector2 _rayDirection)
{
	

	for (const auto& [type, objs] : SCENEMANAGER.get()->currentScene->GetObjectManager()->GetObjects()) {
		// 각 오브젝트들에 대해 순차적으로 검사
		if (Object::ObjectType::Basic == type || Object::ObjectType::UI == type)
		{
			for (const auto& obj : objs) {
				// 레이가 AABB와 교차하는지 확인)
				if (RayIntersectsAABB(_rayOrigin, _rayDirection, obj->boxMin, obj->boxMax)) {
					std::cout << "Ray hit object: " << obj->name << "\n";
					return obj; // obj는 참조형으로 넘어오기 때문에 포인터를 반환해야 합니다.
				}
			}
		}
		else
		{
			continue;
		}
	}
	return nullptr;
}
void ClickHelper::checkClickobj(WPARAM _wParam, WPARAM _lParam)
{
	std::cout << " 클릭 감지됨" << std::endl;
	DXMath::Vector2 rayOrigin;
	//일단 상수 나중에 받아오기
	int screenWidth = 1920;
	int screenHeight = 1080;
	DXMath::Vector2 rayDirection = ScreenToWorldRay(_wParam, _lParam, screenWidth, screenHeight, CameraObject::g_MainCameraObject->GetViewMatrix()
		, CameraObject::g_MainCameraObject->GetProjectionMatrix(), rayOrigin);

	Object* curobj = FindObj(rayOrigin, rayDirection);
	IClickAble* ClickAble = dynamic_cast<IClickAble*>(curobj);
	if (ClickAble)
		ClickAble->OnClick();
}

DXMath::Vector2 ScreenToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const DXMath::Matrix& viewMatrix, const DXMath::Matrix& projectionMatrix, DXMath::Vector2& rayOrigin)
{
	float x = (2.0f * mouseX / screenWidth) - 1.0f;
	float y = 1.0f - (2.0f * mouseY / screenHeight); // Y축은 위쪽이 양수
	DXMath::Vector2 rayClip = DirectX::XMVectorSet(x, y, 1.0f, 1.0f);

	
	DXMath::Matrix invProj = DirectX::XMMatrixInverse(nullptr, projectionMatrix);
	DXMath::Vector2 rayEye = DirectX::XMVector4Transform(rayClip, invProj);
	rayEye = DirectX::XMVectorSet(DirectX::XMVectorGetX(rayEye), // X 값 추출
		DirectX::XMVectorGetY(rayEye), // Y 값 추출
		1.0f,                          // Z 값 설정
		0.0f);                         // W 값 설정

	// DXMath::Vector2로 변환 (필요 시)
	//DXMath::Vector2 rayEye2D = { DirectX::XMVectorGetX(rayEye), DirectX::XMVectorGetY(rayEye) };


	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, viewMatrix);
	DirectX::XMVECTOR rayWorld = DirectX::XMVector3TransformNormal(rayEye, invView);

	rayOrigin = DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), invView);

	// 월드 공간에서의 방향 벡터 정규화
	return DirectX::XMVector3Normalize(rayWorld);
}
bool RayIntersectsAABB(
	DXMath::Vector2 rayOrigin,
	DXMath::Vector2 rayDirection,
	const DXMath::Vector2& boxMin,
	const DXMath::Vector2& boxMax)
{
	if (rayDirection.x == 0.0f && rayDirection.y == 0.0f)
		return false;

	// X, Y 축에 대해 tNear와 tFar 계산
	float tMin = (boxMin.x - rayOrigin.x) / rayDirection.x;
	float tMax = (boxMax.x - rayOrigin.x) / rayDirection.x;

	// tNear, tFar의 순서를 보정
	if (tMin > tMax)
		std::swap(tMin, tMax);

	// Y축에 대해 tNear와 tFar 계산
	float tMinY = (boxMin.y - rayOrigin.y) / rayDirection.y;
	float tMaxY = (boxMax.y - rayOrigin.y) / rayDirection.y;

	// tNear, tFar의 순서를 보정
	if (tMinY > tMaxY)
		std::swap(tMinY, tMaxY);

	// X, Y 축에서의 교차 범위가 겹쳐야 한다
	if (tMin > tMaxY || tMinY > tMax)
		return false;
	std::cout << "tMinX: " << tMin << " tMaxX: " << tMax << std::endl;
	std::cout << "tMinY: " << tMinY << " tMaxY: " << tMaxY << std::endl;
	// 교차 범위가 겹치지 않으면 교차하지 않음
	return true;
}