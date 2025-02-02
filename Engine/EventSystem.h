#pragma once
#include "SingletonBase.h"
#include "Object.h"
class IClickAble;
class IOnMouse;
class Object;
#define eventSysyem EventSystem::GetInstance().get()

DXMath::Ray GenerateRayFromMouse(int mouseX, int mouseY, int screenWidth, int screenHeight,
	const DirectX::XMMATRIX& viewMatrix,
	const DirectX::XMMATRIX& projectionMatrix);

class EventSystem : public SingletonBase<EventSystem>
{
	friend class SingletonBase<EventSystem>;
public:
	EventSystem();
	virtual ~EventSystem() = default;
	Object* FindObj(DXMath::Vector3 _rayOrigin, DXMath::Vector3 _rayDirection);
	void checkClickobj(int _mouseX, int _mouseY); //*****
	void Update(); //온 마우스 확인용
	void OnmouseEvent();
	void BeginDrag(); //꾹 누르는거  일단 나중에 
	void StayDrag();
	void EndDrag();
	POINT startPoint;
	int dragThresholdX;
	int dragThresholdY;
	bool isDragging;
	bool isClick;

private:
	Object* curHolding;
};

