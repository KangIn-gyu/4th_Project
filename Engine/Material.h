#pragma once
#include <assimp/scene.h> 

enum class TextureType   // 어심프 타입 그대로 가져온거
{
    None,                    // 텍스처가 없는 경우, 기본값
    Albedo,                  // 기본 색상 텍스처 (디퓨즈 텍스처와 동일)
    Specular,                // 스펙큘러 텍스처: 반사광을 정의
    Ambient,                 // 주변광 텍스처: 간접광의 밝기를 설정
    Emissive,                // 발광 텍스처: 물체가 자발적으로 빛을 방출
    Height,                  // 높이 맵 텍스처: 표면의 높낮이, 디스플레이스먼트 효과
    Normal,                  // 노멀 맵 텍스처: 표면의 미세한 조명 반응을 변경
    Shininess,               // 광택도 텍스처: 물체 표면의 반사광의 분포
    Opacity,                 // 불투명도 텍스처: 투명도를 정의 (알파 채널)
    Displacement,            // 디스플레이스먼트 맵 텍스처: 표면 기하학적 변형
    LightMap,                // 라이트 맵 텍스처: 정적 조명 정보
    Reflection,              // 반사 텍스처: 물체가 반사하는 환경 텍스처
    Base_Color,              // 기본 색상 텍스처 (Albedo와 동일하지만, PBR에서 주로 사용)
    Normal_Camera,           // 카메라 공간 노멀 맵 텍스처: 카메라 관점에서의 노멀 맵
    Emission_Color,          // 발광 색상 텍스처: 자가 발광 색상을 정의
    Metalness,               // 금속성 텍스처: 물체가 금속인지 아닌지를 정의
    Diffuse_Roughness,       // 디퓨즈와 거칠기 맵 텍스처: 표면의 거칠기 및 색상
    Ambient_occlusion,       // 환경 오클루전 맵 텍스처: 빛 차단 정도
    Sheen,                   // 쉬인 텍스처: 부드러운 광택 효과 (천과 같은 부드러운 표면)
    Clearcoat,               // 클리어코트 텍스처: 보호층을 추가하여 윤기 있는 표면 생성
    Transmission,            // 투과 텍스처: 빛이 물체를 통과하는 정도 (유리, 물 등)
    Unknown                  // 알 수 없는 텍스처: 잘못된 또는 지정되지 않은 텍스처
};

class Texture;
class Shader;
class Material
{
public:
	Material();
    ~Material() {};
	void Load(std::string_view _filePath, aiTextureType _type);  // 어심프에서 로드할때 사용할 예정

    std::string GetName() { return name; }
    void SetName(const std::string_view _materialName) { name = _materialName.data(); } // 디버그 편하게 하기 위해 이름 확인용
    const  std::vector<std::shared_ptr<Texture>>& GetTextures() const { return textures; }
    float GetMetalness() const { return metalness; }
    float GetRoughness() const { return roughness; }

    void SetMetalness(const float _val);
    void SetRoughness(const float _val);

private:

public:

private: 
    std::vector<std::shared_ptr<Texture>> textures;
    std::string name {};
    
    float metalness = 0;
    float roughness = 0;
};

