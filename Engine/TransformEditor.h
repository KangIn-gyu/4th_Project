#pragma once
#include "Editor.h"

class Object;
class TransformEditor : public Editor
{
public:
	TransformEditor();
	~TransformEditor() = default;

	virtual void Initialize()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnEnable() override {};
	virtual void OnDisable() override {};
	virtual void OnDestroy() override {};

	void SetSelectedObject(Object* obj);
private:

public:

private:
	Object* selectedObject = nullptr;
};

