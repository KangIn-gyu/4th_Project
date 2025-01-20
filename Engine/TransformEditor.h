#pragma once
#include "Editor.h"

class Object;
class Mesh;
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

	void SetSelectedObject(Object* _obj);
	void SetSelectedMesh(Mesh* _mesh);
private:

public:

private:
	Object* selectedObject = nullptr;
	Mesh* selectedMesh = nullptr;
};

