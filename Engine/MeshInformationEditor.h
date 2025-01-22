#pragma once
#include "Editor.h"

class Mesh;
class MeshInformationEditor : public Editor
{
public:
	MeshInformationEditor();
	~MeshInformationEditor() = default;

	virtual void Initialize()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnEnable() override {};
	virtual void OnDisable() override {};
	virtual void OnDestroy() override {};

	void SetSelectedMesh(Mesh* _mesh);
private:

public:

private:
	Mesh* selectedMesh = nullptr;
};

