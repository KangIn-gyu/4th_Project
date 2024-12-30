#pragma once
#include "IResources.h"

enum class TextureType
{
    Albedo,  // 디퓨즈랑 같음
    Normal,
    Metallic,
    Roughness,
    Opacity,
    Emissive,
    End
};

class Texture;
class Shader;
class Material : public IResources
{
public:
    using TextureUnMap = std::unordered_map<std::wstring, std::shared_ptr<Texture>>;

	Material() = default;
	virtual ~Material() = default;
	virtual void Load(std::wstring_view _filePath) override;  // 어심프에서 로드할때 사용할 예정

    void SetShader(std::wstring_view _filePath);

private:
    TextureType GetTextureTypeFromFileName(const std::wstring& fileName);

public:

private: 
    std::unordered_map<TextureType, TextureUnMap> textures;
    std::shared_ptr<Shader> shader;

};

