#pragma once
#include "Editor.h"

class MaterialInformationEditor : public Editor
{
public:
	MaterialInformationEditor();
	~MaterialInformationEditor() = default;

	virtual void Initialize()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnEnable() override {};
	virtual void OnDisable() override {};
	virtual void OnDestroy() override {};

private:

public:

private:

};

