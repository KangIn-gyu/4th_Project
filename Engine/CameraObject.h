#pragma once
#include "Object.h"

struct WindowInfo;
class ConstantBuffer;
class CameraObject : public Object
{
public:
	CameraObject(std::string_view _name , Object::ObjectType type = Object::ObjectType::Camera);
	virtual ~CameraObject() {};

	virtual void Initialize() override;
	virtual void Update(const float _deltaTime);
	virtual void FixedUpdate() override {};
	virtual void LateUpdate() override {};

	std::pair<int, int> GetWindowSize() const;
	DXMath::Matrix GetProjectionMatrix();
	DXMath::Matrix GetViewMatrix();

	static CameraObject* g_MainCameraObject;
private:

public:

private:
	WindowInfo* windowInfo {}; // 여기서 삭제할 필요 없음 어차피 윈도우에서 삭제함.

};
