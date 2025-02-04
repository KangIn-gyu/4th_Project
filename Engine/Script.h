#pragma once
#include "TransformComponent.h"
#include "CameraCompoent.h"
#include "RenderComponent.h"
#include "D2DRenderComponent.h"
#include "ModelComponent.h"
#include "BoxCollider.h"

class Objcet;
class Script
{
public:
	Script(Objcet* _ownerObjcet) { ownerObjcet = _ownerObjcet; }
	virtual ~Script() {}

	virtual void ComponentSetting() = 0;                       // 컴포넌트가 다 생성된 이후 수치 조정하는것
	virtual void ComponentUpdate(const float _deltaTime) = 0;  // 업데이트

protected:
	Objcet* ownerObjcet; // 이걸 생성한 오브젝트가 주인이다.
};

