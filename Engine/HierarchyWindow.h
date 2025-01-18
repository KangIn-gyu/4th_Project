#pragma once
#include "EditorWindow.h"

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
private:

public:

private:

};

