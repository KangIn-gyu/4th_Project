#pragma once
#include "IResources.h"

class Texture : public IResources
{
public:
	Texture() = default;
	~Texture() = default;

	virtual void Load(std::wstring filePath) override;
	ComPtr<ID3D11ShaderResourceView> GetTexture() { return texture; }
private:

public:

public:
	ComPtr<ID3D11ShaderResourceView> texture;

};

