#include "pch.h"
#include "Material.h"
#include "Texture.h"   // 헤더 꼬임 일어날 수 있음 (주의) 아직 테스트 안해봄
#include "Shader.h"    // 헤더 꼬임 일어날 수 있음 (주의)
#include "ResourceSystem.h"

static std::unordered_map<aiTextureType, TextureType> typeMapping = // 매핑용
{
    { aiTextureType_DIFFUSE, TextureType::Albedo },
    { aiTextureType_SPECULAR, TextureType::Specular },
    { aiTextureType_AMBIENT, TextureType::Ambient },
    { aiTextureType_EMISSIVE, TextureType::Emissive },
    { aiTextureType_HEIGHT, TextureType::Height },
    { aiTextureType_NORMALS, TextureType::Normal },
    { aiTextureType_SHININESS, TextureType::Shininess },
    { aiTextureType_OPACITY, TextureType::Opacity },
    { aiTextureType_DISPLACEMENT, TextureType::Displacement },
    { aiTextureType_LIGHTMAP, TextureType::LightMap },
    { aiTextureType_REFLECTION, TextureType::Reflection },
    { aiTextureType_UNKNOWN, TextureType::Unknown }  // 기본 값
};
// 하나로 돌려 쓰자 어차피 이거 다른 곳에서 공유한다고 해도 의미도 없고 각 메테리얼 마다 맵 들고 있으면 낭비니깐
// 이럴 경우에는 언맵으로 찾는게 스위치문 보다 빠르다

Material::Material()
{

}

void Material::Load(std::string_view _filePath, aiTextureType _type)
{
    auto it = typeMapping.find(_type);
    if (it != typeMapping.end())
    {
        upLoadType = it->second;
    }
    else
    {
        upLoadType = TextureType::Unknown;
    }

    textures.emplace_back(upLoadType, RESOURCESYSTEM->Load<Texture>(_filePath));
    upLoadType = TextureType::Unknown;
}



