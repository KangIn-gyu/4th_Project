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
// Shadow Helper Functions
//--------------------------------------------------------------------------------------
float CalculateShadow(PixelInputType input)
{
    float3 projCoords = input.LightSpacePos.xyz / input.LightSpacePos.w;
    
    // 범위 체크
    if (projCoords.x < -1.0f || projCoords.x > 1.0f ||
        projCoords.y < -1.0f || projCoords.y > 1.0f ||
        projCoords.z < 0.0f || projCoords.z > 1.0f)
    {
        return 1.0f;
    }
    
    // NDC 좌표를 UV 좌표로 변환
    projCoords.x = projCoords.x * 0.5 + 0.5;
    projCoords.y = -projCoords.y * 0.5 + 0.5;
    
    // depth test 개선
    float currentDepth = projCoords.z;
    float bias = 0.0025; // 고정 bias 사용

    // 거리에 따른 PCF 커널 크기 조정
    float viewDistance = length(input.worldPos.xyz - eyePosition);
    int sampleRange = PCF_SAMPLES;
    float shadow = 0.0;
    float2 texelSize = 1.0f / float2(4096.0f, 4096.0f);
    
    // 개선된 PCF 필터링
    [unroll]
    for (int x = -sampleRange; x <= sampleRange; ++x)
    {
        [unroll]
        for (int y = -sampleRange; y <= sampleRange; ++y)
        {
            float2 offset = float2(x, y) * texelSize * 0.5; // PCF 범위 축소
            shadow += shadowMap.SampleCmpLevelZero(
                samPoint,
                projCoords.xy + offset,
                currentDepth - bias
            );
        }
    }
    
    int samples = (2 * sampleRange + 1) * (2 * sampleRange + 1);
    shadow /= samples;
    
    // 그림자 강도 조절
    shadow = shadow * 0.95 + 0.05; // 완전히 검은 그림자 방지
    
    return shadow;
}