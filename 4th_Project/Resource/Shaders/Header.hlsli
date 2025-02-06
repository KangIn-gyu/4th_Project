#include "TextureHeader.hlsli"
#include "SamplerHeader.hlsli" 

#define GAMMA 2.2f
static const float PI = 3.14159265359;
#define MIPMAP_LEVELS 6.0f
#define PCF_SAMPLES 2   // PCF 샘플링 범위 (3은 7x7, 2는 5x5, 1은 3x3)

// 0 ~ 13 (슬롯 번호) // (4096 *) 16 byte 단위로 정렬해야 한다.
cbuffer MatrixBuffer : register(b0) // ConstantBuffer 슬롯
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer ObjectBuffer : register(b1) // ConstantBuffer 슬롯
{
    float metalness;
    float roughness;
    int   onOutline;
};

cbuffer CameraBuffer : register(b2)
{
    float3 eyePosition;
    float3 lightDirection;
}

cbuffer MatrixPallete : register(b3)
{
    matrix MatrixPalleteArray[400];
}

cbuffer ShadowBuffer : register(b4)
{
    matrix lightViewProj;
}

cbuffer ProductBuffer : register(b5)
{
    float totalTime;
}

struct SpotLightData
{
    float3 position; // 광원의 위치
    float pad1;
    float3 direction; // 광원이 비추는 방향
    float pad2;
    float3 color; // 광원의 색상
    float range; // 광원의 영향 범위
    float innerCone; // 내부 콘의 코사인 값
    float outerCone; // 외부 콘의 코사인 값
    float intensity; // 광원의 강도
    float pad3;
};

cbuffer LightBuffer : register(b6)
{
    SpotLightData spotLights[7];
    int LIGHT_NUM;
}

struct VertexInputType
{
//    uint   VertexID : SV_VertexID; // 정점 ID
    float4 Position    : POSITION;    // 정점 위치
    float4 Color       : COLOR;       // 색상
    float2 TexCoord    : TEXCOORD0;   // 텍스처 좌표
    float3 Normal      : NORMAL;      // 노말
    float3 Tangent     : TANGENT;     // 탄젠트 벡터
    float3 Binormal    : BINORMAL;    // 바이노멀 벡터
    uint4 BlendIndices : BLENDINDICES; // 정점이 영향을 받는 본 인덱스
    float4 BlendWeight : BLENDWEIGHT; // 정점이 본으로부터 받는 가중치
};

struct PixelInputType
{
//    uint   VertexID : SV_VertexID; // 정점 ID
    float4 Position : SV_POSITION; // 정점 위치
    float4 Color    : COLOR;       // 색상
    float2 TexCoord : TEXCOORD0;   // uv
    float4 worldPos : TEXCOORD1;   // 월드
    float3 Normal   : NORMAL;      // 노말
    float3 Tangent  : TANGENT;     // 탄젠트 벡터
    float3 Binormal : BINORMAL;    // 바이노멀 벡터
    float4 LightSpacePos : TEXCOORD2;
};

struct VS_SKYBOX_INPUT
{
    float3 Pos : POSITION;
};

struct VS_SKYBOX_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 TexCoord : TEXCOORD0;
};

// 일단 하나로 통일해서 보기 쉽게 처리를 함