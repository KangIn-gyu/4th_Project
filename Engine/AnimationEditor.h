#pragma once
#include "Editor.h"

class Object;
class AnimationEditor : public Editor
{
public:
	AnimationEditor();
	~AnimationEditor() = default;

	virtual void Initialize()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnEnable() override; // 활성화
	virtual void OnDisable() override; // 사용 안함
	virtual void OnDestroy() override {};

	void SetSelectedObject(Object* _obj);
private:

public:

private:
	Object* selectedObject = nullptr;
};

