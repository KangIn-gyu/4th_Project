#pragma once
#include "EditorWindow.h"

class ObjectManager;
class Object;
class AiNode;
class Model;
class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow();
	~HierarchyWindow();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Run() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

	void SetObjectManager(ObjectManager* _objectManager);
	void DrawNodeRecursive(std::shared_ptr<Model> _model, AiNode* _node);
private:


public:

private:
	ObjectManager* objectManager; // 여기 있는 포인터는 가르키기만 하고 지우는건 씬 자체에서 알아서 함.
	Object* selectedObject = nullptr;
};

