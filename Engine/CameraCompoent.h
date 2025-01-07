#pragma once
#include "Component.h"
#include "DirectXInput.h"

struct CameraInfo;
class CameraCompoent : public Component , public IinputProcesser
{
public:
	CameraCompoent();
	virtual ~CameraCompoent();

	virtual void ComponentInitialize() override;
	virtual void ComponentUpdate(const float _deltaTime) override;

	DXMath::Matrix GetProjectionMatrix() const { return projectionMatrix; }
	DXMath::Matrix GetViewMatrix() const;

	void SetProjection(float _FovAngleY, float _Near, float _Far);
	void SetSpeed(const float _speed);
	void SetRotationSpeed(const float _speed);

	virtual void OnInputProcess(const DX::Keyboard::State& KeyState,
		const DX::Keyboard::KeyboardStateTracker& KeyTracker,
		const DX::Mouse::State& MouseState,
		const DX::Mouse::ButtonStateTracker& MouseTracker) override;

	DXMath::Vector3 GetForward();
	DXMath::Vector3 GetRight();

private:
	void UpdateViewMatrix();
	void AddInputVector(const DXMath::Vector3& input);

public:

private:
	float clientWidth  {};
	float clientHeight {};

	CameraInfo* cameraInfo{};

	DXMath::Vector3 InputVector {};

	DXMath::Matrix projectionMatrix {};
	DXMath::Matrix viewMatrix {};
};

