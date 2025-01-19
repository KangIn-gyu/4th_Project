#pragma once
#include "EditorWindow.h"

class Editor;
class Object;
class InspectorWindow : public EditorWindow
{
public:
	InspectorWindow();
	~InspectorWindow();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Run() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

	void SetSelectedObject(Object* obj); 
private:

public:

private:
	Object* selectedObject;
	std::vector<Editor*> editors;
};

// °í¹Î »çÇ× ½Ì±ÛÅæÀ¸·Î ¸¸µé¾î¾ß ÇÏ³ª.. °í¹ÎÁß