#pragma once
#include "Editor.h"

class Texture;
class TextureInformationEditor : public Editor
{
public:
	TextureInformationEditor();
	~TextureInformationEditor() = default;

	virtual void Initialize()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnEnable() override {};
	virtual void OnDisable() override {};
	virtual void OnDestroy() override {};

	void SetSelectedTexture(Texture* _texture);
private:

public:

private:
	Texture* texture;
};

