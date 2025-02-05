#include "Header.hlsli"

//--------------------------------------------------------------------------------------
// Helper Functions
//--------------------------------------------------------------------------------------
float4 SampleBaseColorWithGamma(Texture2D colorMap, float2 uv, float4 defaultColor)
{
    
    float4 sampledColor = colorMap.Sample(samLinear, uv);
    
    // basecolor map이 없으면 흰색
    if (all(sampledColor == 0))
    {
        return float4(1, 1, 1, 1);
    }
    // Gamma Decode
    return float4(pow(sampledColor.rgb, GAMMA), sampledColor.a);
}

float3x3 GetTangentSpace(float3 normal, float3 tangent)
{
    float3 N = normalize(normal);
    float3 T = normalize(tangent);
    float3 B = normalize(cross(N, T)); // 이 순서가 맞는지 확인
    T = normalize(cross(B, N)); // T 재계산으로 직교성 보장
    
    return float3x3(T, B, N); // 행렬 구성 순서 확인
}

//--------------------------------------------------------------------------------------
// PBR Helper Functions
//--------------------------------------------------------------------------------------
float3 F_Schlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float D_GGX(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}

float G_SchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float G_Smith(float NdotV, float NdotL, float roughness)
{
    return G_SchlickGGX(NdotV, roughness) * G_SchlickGGX(NdotL, roughness);
}

//--------------------------------------------------------------------------------------
// IBL Helper Functions
//--------------------------------------------------------------------------------------
float3 GetIBLIrradiance(float3 N)
{
    return IrradianceMap.Sample(samLinear, N).rgb;
}

float3 GetIBLRadiance(float3 N, float3 V, float roughness)
{
    float3 R = reflect(-V, N);
    
    // roughness가 0에 가까울 때는 가장 선명한 반사를 보여주기 위해
    // mip level을 0으로 설정
    float mipLevel = roughness * roughness * MIPMAP_LEVELS;
    
    // 완벽한 반사를 위해 환경맵 직접 사용
    return EnvironmentMap.SampleLevel(samLinear, R, mipLevel).rgb;
}

float2 IntegrateBRDF(float NdotV, float roughness)
{
    return BRDFLUT.Sample(samLinear, float2(NdotV, roughness)).rg;
}

//--------------------------------------------------------------------------------------
// Tone Mapping Helper Functions
//--------------------------------------------------------------------------------------

// Uncharted 2 톤매핑 - 게임용으로 최적화된 톤매핑
float3 Uncharted2ToneMapping(float3 color)
{
    float A = 0.15; // 숄더 형태 조절
    float B = 0.50; // 대비 조절
    float C = 0.10; // 선형 부분 조절
    float D = 0.20; // 토 형태 조절
    float E = 0.02; // 블랙 레벨
    float F = 0.30; // 전체 밝기 스케일
    float W = 11.2; // 화이트포인트 값
   
   // 메인 톤매핑 커브 계산
    float3 curr = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
   
   // 화이트포인트 기준 스케일 계산
    float3 whiteScale = 1.0f / (((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F);
   
   // 최종 색상 반환
    return curr * whiteScale;
}

// ACES Filmic 톤매핑 - 영화 산업 표준의 톤매핑 커브
float3 ACESFilmicToneMapping(float3 color)
{
    float a = 2.51f; // 숄더(밝은 부분) 조절
    float b = 0.03f; // 토(어두운 부분) 조절
    float c = 2.43f; // 미드톤 대비 조절
    float d = 0.59f; // 전체 밝기 조절
    float e = 0.14f; // 블랙 레벨 조절
   
   // ACES 근사값 공식 적용 후 0~1 범위로 클램프
    return saturate((color * (a * color + b)) / (color * (c * color + d) + e));
}

//--------------------------------------------------------------------------------------
// Rim Light Helper Functions
//--------------------------------------------------------------------------------------

float3 RimLight(float3 normal, float3 viewDir, float rimPower, float3 rimColor)
{
    float rim = 1.0 - saturate(dot(normalize(viewDir), normal));
    float rimL = pow(rim, rimPower);
    
    return rimL * rimColor;
}

float3 CardSelectionRimLight(float3 normal, float3 viewDir, float3 rimColor)
{
    // 기본 프레넬 림라이트 계산 - 얼마나 각 픽셀이 외곽에 가까운지
    // 각 픽셀이 얼마나 rimlight효과를 받아야 하는지 결정하는 계수
    float rimFactor = 1.0 - max(dot(normal, viewDir), 0.0);
    
    // 림라이트 강화를 위한 파라미터
    float rimPower = 2.0;     //  낮을수록 더 넓은 영역에 림라이트가 적용됨
    float rimStrength = 12.0; // 림라이트 강도

    // 위치에 따른 효과 계산
   //float xPos = dot(normal, float3(1, 0, 0));
   //float yPos = dot(normal, float3(0, 1, 0));
   //
   //// 시간과 위치 기반 펄스
   //float pulseSpeed = 10.0; // 밝기 변화 속도
   //float positionOffset = xPos * 15.0 + yPos * 13.0;
   //float pulse = sin(totalTime * pulseSpeed + positionOffset);
   //pulse = (pulse * 0.5 + 0.5) * 0.5 + 0.5;
    
    // 시간 기반 펄스
    float pulseSpeed = 4.0; // 숨쉬는 속도 (낮출수록 천천히 변함)
    float pulse = sin(totalTime * pulseSpeed);
    pulse = (pulse * 0.5 + 0.5); // 0~1 범위로 변환 ( 음수값 제거 위해)
    
    // 향상된 림라이트 계산 - 림파워가 높을수록 외곽선이 날카로워짐
    rimFactor = pow(rimFactor, rimPower);
    
    // threshold + 0.2 == 림라이트 그라데이션의 끝점을 결정 60% 지점에서 최대 강도 도달
     // 임계값 기반 강화 - 펄스에 따라 임계값 변화
    float baseThreshold = 0.4;  // 림라이트가 시작되는 임계값 ( 낮을수록 일찍시작됨 ) 림팩터가 40% 이상인 지점부터 림라이트가 시작됨
    float thresholdRange = 0.2; // 임계값 변화 범위
    // pulse가 0 일때 0.4 pulse가 1 일때 0.6을 가지게끔 하는 과정 threshold는 0.4 ~ 0.6 사이를 시간에 따라 변화됨
    float threshold = baseThreshold + (pulse * thresholdRange); // 임계값이 시간에 따라 변화
    // 특정 임계값 기준(threshold) 으로 rim효과를 부드럽게
    // 그러므로 0.4 ~ 0.55 에서 0.6 ~ 0.75 사이를 순회
    rimFactor = smoothstep(threshold, threshold + 0.15, rimFactor);
    
    //rimStrength로 전체적인 강도를 조절하고
    //pulse로 시간에 따른 변화를 줌
    rimFactor *= rimStrength * pulse;
    
    // 부드러운 가장자리를 위한 스무딩 (두 값이 차이가 클수록 부드러워지고 작을수록 날카로워짐 또한 두 값이 모두 낮다면 더 넓은 영역에 림라이트가 적용됨)
    // 림라이트의 강도를 0~1 사이로 정규화 하는 역할
    // 강도가 0.2 미만이면 0 반환 0.6 초과면 1 반환
    float smoothFactor = smoothstep(0.2, 0.6, rimFactor);
    
    // 최종 림라이트 색상
    return rimColor * smoothFactor;
}

// 라플라시안 필터 상수
static const float mask[9] =
{
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
};
                              
static const float2 offsets[9] =
{
    float2(-1, -1), float2(0, -1), float2(1, -1),
    float2(-1, 0), float2(0, 0), float2(1, 0),
    float2(-1, 1), float2(0, 1), float2(1, 1)
};

float GetLaplacianEdge(float3 centerNormal, float3 tangent, float2 texCoord)
{
    float3 normalSum = float3(0, 0, 0);
    float texelSize = 1.5f / 1024.0f; // 텍스처 크기에 따라 조정
    
    // TBN 행렬 생성 시 전달받은 tangent 사용
    float3x3 TBN = GetTangentSpace(centerNormal, tangent);
    
    // 중심 노말을 월드 스페이스로 변환
    float4 centerNormalSample = NormalMap.Sample(samLinear, texCoord);
    float3 centerWorldNormal = normalize(mul(centerNormalSample.rgb * 2.0f - 1.0f, TBN));
    
    // 확장된 라플라시안 처리
    float edgeIntensity = 0.0f;
    
    for (int i = 0; i < 9; i++)
    {
        float2 offset = offsets[i] * texelSize;
        float4 normalSample = NormalMap.Sample(samLinear, texCoord + offset);
        float3 tangentNormal = normalSample.rgb * 2.0f - 1.0f;
        float3 worldNormal = normalize(mul(tangentNormal, TBN));
        
        // 노말 차이 강화
        float normalDiff = 1.0 - dot(centerWorldNormal, worldNormal);
        edgeIntensity += normalDiff * abs(mask[i]);
    }
    
    // 엣지 강화
    float threshold = 0.3; // 임계값 조정
    edgeIntensity = smoothstep(threshold, threshold + 0.1, edgeIntensity);
    
    
    // 추가 선명도
    edgeIntensity = pow(edgeIntensity, 1.5);
    
    return saturate(edgeIntensity);
}

//--------------------------------------------------------------------------------------
// Spot Light Helper Functions
//--------------------------------------------------------------------------------------

float3 CalculateSpotLight(SpotLightData light, float3 worldPos, float3 N, float3 V,
                         float3 baseColor, float metallic, float roughness, float3 F0)
{
    float3 L = normalize(light.position - worldPos);
    float3 H = normalize(V + L);
    
    float distance = length(light.position - worldPos);
    float attenuation = 1.0 - saturate(distance / light.range);
    attenuation = attenuation * attenuation;
    
    // Spot light cone calculation
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerCone - light.outerCone;
    float spotIntensity = saturate((theta - light.outerCone) / epsilon);
    
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0001);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);
    
    // PBR 계산
    float D = D_GGX(NdotH, max(0.1f, roughness));
    float3 F = F_Schlick(HdotV, F0);
    float G = G_Smith(NdotV, NdotL, roughness);
    
    float3 specular = (D * F * G) / (4.0 * NdotV * NdotL + 0.0001);
    float3 kD = (1.0 - F) * (1.0 - metallic);
    
    float3 diffuse = kD * baseColor / PI;
    
    return (diffuse + specular) * light.color * light.intensity * NdotL * attenuation * spotIntensity;
}