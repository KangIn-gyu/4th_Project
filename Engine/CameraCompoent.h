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
	void MovingFlag(bool _flag) { movingFlag = _flag; }
	void LookAt(const DXMath::Vector3& _targetPosition);


	virtual void OnInputProcess(const DX::Keyboard::State& _KeyState,
		const DX::Keyboard::KeyboardStateTracker& _KeyTracker,
		const DX::Mouse::State& _MouseState,
		const DX::Mouse::ButtonStateTracker& _MouseTracker) override;

	DXMath::Vector3 GetForward();
	DXMath::Vector3 GetRight();

	CameraInfo* GetCameraInfo() { return cameraInfo; }
private:
	void UpdateViewMatrix();
	void AddInputVector(const DXMath::Vector3& input);
	void UpdateProjection();

public:
	bool title = false;

private:
	float clientWidth  {};
	float clientHeight {};
	DXMath::Vector3 lookat{};
	CameraInfo* cameraInfo{};

	DXMath::Vector3 InputVector {};

	DXMath::Matrix projectionMatrix {};
	DXMath::Matrix viewMatrix {};

	bool movingFlag = true; // TODO : true로 되어 있으면 수정해야됨 
};

