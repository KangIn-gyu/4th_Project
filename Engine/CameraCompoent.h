#pragma once
#include "Component.h"

struct CameraInfo;
class CameraCompoent : public Component
{
public:
	CameraCompoent();
	virtual ~CameraCompoent();
private:

public:
	virtual void ComponentInitialize() override;
	virtual void ComponentUpdate(const float _deltaTime) override {};

	DXMath::Matrix GetProjectionMatrix() const { return projectionMatrix; }
	void SetProjection(float _FovAngleY, float _Near, float _Far);

private:
	float clientWidth  {};
	float clientHeight {};

	CameraInfo* cameraInfo{};

	DXMath::Vector3 InputVector{};

	DXMath::Matrix projectionMatrix {};
	DXMath::Matrix viewMatrix {};
};

