#pragma once
#include "EditorWindow.h"

class Editor;
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

private:

public:

private:
	std::vector<Editor*> editors;
};

