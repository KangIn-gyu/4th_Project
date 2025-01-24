#include "pch.h"
#include "Material.h"
#include "Texture.h"   // 헤더 꼬임 일어날 수 있음 (주의) 아직 테스트 안해봄
#include "Shader.h"    // 헤더 꼬임 일어날 수 있음 (주의)
#include "ResourceSystem.h"

static std::unordered_map<aiTextureType, std::pair<TextureType, int>> typeMapping = // 매핑용
{
    { aiTextureType_DIFFUSE,           {TextureType::Albedo, 0} },              // 기본 색상 텍스처
    { aiTextureType_SPECULAR,          {TextureType::Specular, 1} },            // 스펙큘러 텍스처
    { aiTextureType_AMBIENT,           {TextureType::Ambient, 2} },             // 주변광 텍스처
    { aiTextureType_EMISSIVE,          {TextureType::Emissive, 3} },            // 발광 텍스처
    { aiTextureType_HEIGHT,            {TextureType::Height, 4} },              // 높이 맵 텍스처
    { aiTextureType_NORMALS,           {TextureType::Normal, 5} },              // 노멀 맵 텍스처
    { aiTextureType_SHININESS,         {TextureType::Shininess, 6} },           // 광택도 텍스처
    { aiTextureType_OPACITY,           {TextureType::Opacity, 7} },             // 불투명도 텍스처
    { aiTextureType_DISPLACEMENT,      {TextureType::Displacement, 8} },        // 디스플레이스먼트 맵 텍스처
    { aiTextureType_LIGHTMAP,          {TextureType::LightMap, 9} },            // 라이트 맵 텍스처
    { aiTextureType_REFLECTION,        {TextureType::Reflection, 10} },         // 반사 텍스처
    { aiTextureType_BASE_COLOR,        {TextureType::Base_Color, 11} },          // PBR 기본 색상 텍스처
    { aiTextureType_NORMAL_CAMERA,     {TextureType::Normal_Camera, 12} },      // 카메라 공간 노멀 맵
    { aiTextureType_EMISSION_COLOR,    {TextureType::Emission_Color, 13} },     // 발광 색상 텍스처
    { aiTextureType_METALNESS,         {TextureType::Metalness, 14} },          // 금속성 텍스처
    { aiTextureType_DIFFUSE_ROUGHNESS, {TextureType::Diffuse_Roughness, 15} },  // 디퓨즈와 거칠기 맵
    { aiTextureType_AMBIENT_OCCLUSION, {TextureType::Ambient_occlusion, 16} },  // 환경 오클루전 맵
    { aiTextureType_SHEEN,             {TextureType::Sheen, 17} },              // 쉬인 텍스처
    { aiTextureType_CLEARCOAT,         {TextureType::Clearcoat, 18} },          // 클리어코트 텍스처
    { aiTextureType_TRANSMISSION,      {TextureType::Transmission, 19} },       // 투과 텍스처
    { aiTextureType_UNKNOWN,           {TextureType::Unknown, -1} }             // 알 수 없는 텍스처
};
// 하나로 돌려 쓰자 어차피 이거 다른 곳에서 공유한다고 해도 의미도 없고 각 메테리얼 마다 맵 들고 있으면 낭비니깐
// 이럴 경우에는 언맵으로 찾는게 스위치문 보다 빠르다

Material::Material()
{

}

void Material::Load(std::string_view _filePath, aiTextureType _type)
{
    TextureType upLoadType = TextureType::Unknown;
    int typeIndex = -1;

    auto it = typeMapping.find(_type);
    if (it != typeMapping.end())
    {
        upLoadType = it->second.first;
        typeIndex = it->second.second;
    }
    else
    {
        upLoadType = TextureType::Unknown;
    }

    std::shared_ptr<Texture> texture = RESOURCESYSTEM->Load<Texture>(_filePath);
    texture->SetTextureTypeIndex(typeIndex);
    texture->SetName(_filePath);
    textures.emplace_back(texture);
}

void Material::SetMetalness(const float _val)
{
    metalness = _val;
}

void Material::SetRoughness(const float _val)
{
    roughness = _val;
}





