#pragma once
#include "EditorWindow.h"
#include "SingletonBase.h"

#define INSPECTOR InspectorWindow::GetInstance()

class Editor;
class Object;
class Mesh;
class Texture;
class AiNode;
class InspectorWindow : public EditorWindow , public SingletonBase<InspectorWindow>
{
	friend class SingletonBase<InspectorWindow>;
public:
	virtual ~InspectorWindow();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Run() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

	void SetSelectedObject(Object* _obj); 
	void SetSelectedMesh(Mesh* _mesh);
	void SetSelectedTexture(Texture* _texture);
	void SetSelectedAiNode(AiNode* _aiNode);
private:
	InspectorWindow();

public:
	Object* selectedObject;
	Mesh* selectedMesh;
	Texture* selectedTexture;
	AiNode* selectedAiNode;
private:
	std::vector<Editor*> editors;
};
