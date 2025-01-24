#pragma once
#include "IResources.h"

class Texture : public IResources
{
public:
	Texture() = default;
	virtual ~Texture() = default;

	virtual void Load(std::string_view _filePath) override;
	ComPtr<ID3D11ShaderResourceView> GetTexture() { return texture; }
	void SetTextureTypeIndex(int _index) { textureTypeIndex = _index; }
	int GetTextureTypeIndex() { return textureTypeIndex; }
	std::string GetName() { return name; }
	void SetName(std::string_view _naem);
private:

public:

public:
	std::string name;
	ComPtr<ID3D11ShaderResourceView> texture;
	int textureTypeIndex = -1; // -1 일경우 오류
};

