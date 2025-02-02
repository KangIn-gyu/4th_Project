#pragma once
#include "IResources.h"

class Texture : public IResources
{
public:
	Texture() = default;
	virtual ~Texture() = default;

	virtual void Load(std::string_view _filePath) override;
	ComPtr<ID3D11ShaderResourceView> GetTexture() { return texture; }
	void SetTextureTypeIndex(int _index) { textureTypeIndex.insert(_index); }
	std::set<int> GetTextureTypeIndexs() { return textureTypeIndex; }
	const std::string& GetName() { return name; }
	void SetName(std::string_view _naem);
private:

public:

public:
	std::string name;
	ComPtr<ID3D11ShaderResourceView> texture;
	std::set<int> textureTypeIndex;
};

