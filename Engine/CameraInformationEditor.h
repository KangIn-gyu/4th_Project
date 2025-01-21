#pragma once
#include "Editor.h"

class CameraObject;
class Object;
class CameraInformationEditor : public Editor
{
public:
	CameraInformationEditor();
	~CameraInformationEditor() = default;

	virtual void Initialize()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override {};

	void SetCameraObject(Object* _camera);
private:

public:

private:
	CameraObject* selectedcamera;
};

