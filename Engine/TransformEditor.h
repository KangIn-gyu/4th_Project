#pragma once
#include "Editor.h"

class Object;
class AiNode;
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
	void SetSelectedNode(AiNode* _node);
private:

public:

private:
	Object* selectedObject = nullptr;
	AiNode* selectedNode = nullptr;
};

