#pragma once
#include "IResources.h"

class Texture : public IResources
{
public:
	Texture() = default;
	virtual ~Texture() = default;

	virtual void Load(std::string_view _filePath) override;
	ComPtr<ID3D11ShaderResourceView> GetTexture() { return texture; }
private:

public:

public:
	ComPtr<ID3D11ShaderResourceView> texture;
	
};

