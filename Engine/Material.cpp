#include "pch.h"
#include "Material.h"
#include "Texture.h"   // 헤더 꼬임 일어날 수 있음 (주의) 아직 테스트 안해봄
#include "Shader.h"    // 헤더 꼬임 일어날 수 있음 (주의)
#include "ResourceSystem.h"

void Material::Load(std::wstring_view _filePath)
{
	// 여기다 텍스쳐 로드를 만들어야 한다
    TextureType type = GetTextureTypeFromFileName(_filePath.data());
	// textures.emplace(type, RESOURCESYSTEM->Load<Texture>(_filePath)); 
    auto& textureMap = textures[type];
    if (textureMap.find(_filePath.data()) == textureMap.end())
    { // 텍스쳐 없는 경우
       auto loadedTexture = RESOURCESYSTEM->Load<Texture>(_filePath);
       if (nullptr != loadedTexture)
       {
           textureMap.emplace(_filePath.data(), loadedTexture);
       }
       else
       {
           // 추후 생각하자
       }
    }
    else
    {
        // 추후 로그 시스템 추가하기 
    }
    
}

void Material::SetShader(std::wstring_view _filePath)
{
    shader = RESOURCESYSTEM->Load<Shader>(_filePath);
}

TextureType Material::GetTextureTypeFromFileName(const std::wstring& fileName)
{
    if (fileName.find(L"Albedo") != std::wstring::npos) return TextureType::Albedo;
    if (fileName.find(L"Normal") != std::wstring::npos) return TextureType::Normal;
    if (fileName.find(L"Metallic") != std::wstring::npos) return TextureType::Metallic;
    if (fileName.find(L"Roughness") != std::wstring::npos) return TextureType::Roughness;
    if (fileName.find(L"Opacity") != std::wstring::npos) return TextureType::Opacity;
    if (fileName.find(L"Emissive") != std::wstring::npos) return TextureType::Emissive;
    return TextureType::End;
}
