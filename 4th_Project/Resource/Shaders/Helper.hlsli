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

// 기타 렌더링 연산 함수

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

float3 RimLight(float3 normal, float3 viewDir, float rimPower, float3 rimColor)
{
    float rim = 1.0 - saturate(dot(normalize(viewDir), normal));
    float rimL = pow(rim, rimPower);
    
    return rimL * rimColor;
}

float3 CardSelectionRimLight(float3 normal, float3 viewDir, float3 rimColor)
{
    // 기본 프레넬 림라이트 계산
    float rimFactor = 1.0 - max(dot(normal, viewDir), 0.0);
    
    // 림라이트 강화를 위한 파라미터
    float rimPower = 3.0; // 림라이트 선명도
    float rimStrength = 2.0; // 림라이트 강도
    float pulseSpeed = 10.0; // 밝기 변화 속도
    
    // 시간에 따른 펄스 효과
    float pulse = (sin(totalTime * pulseSpeed) * 0.5 + 0.5) * 0.3 + 0.7;
    
    // 향상된 림라이트 계산
    rimFactor = pow(rimFactor, rimPower);
    rimFactor *= rimStrength * pulse;
    
    // 부드러운 가장자리를 위한 스무딩
    float smoothFactor = smoothstep(0.0, 1.0, rimFactor);
    
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
    float texelSize = 1.0f / 1024.0f; // 텍스처 크기에 따라 조정
    
    // TBN 행렬 생성 시 전달받은 tangent 사용
    float3x3 TBN = GetTangentSpace(centerNormal, tangent);
    
    // 중심 노말을 월드 스페이스로 변환
    float4 centerNormalSample = NormalMap.Sample(samLinear, texCoord);
    float3 centerWorldNormal = normalize(mul(centerNormalSample.rgb * 2.0f - 1.0f, TBN));
    
    // 라플라시안 필터 적용
    for (int i = 0; i < 9; i++)
    {
        float2 offset = offsets[i] * texelSize;
        float4 normalSample = NormalMap.Sample(samLinear, texCoord + offset);
        float3 tangentNormal = normalSample.rgb * 2.0f - 1.0f;
        float3 worldNormal = normalize(mul(tangentNormal, TBN));
        
        normalSum += worldNormal * mask[i];
    }
    
    // 엣지 강도 계산
    float edgeIntensity = length(normalSum);
    return saturate(edgeIntensity);
}
