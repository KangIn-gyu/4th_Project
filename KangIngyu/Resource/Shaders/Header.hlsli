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
};

cbuffer CameraBuffer : register(b2)
{
    float3 eyePosition;
    float3 lightDirection;
}

struct VertexInputType
{
//    uint   VertexID : SV_VertexID; // 정점 ID
    float4 Position : POSITION;    // 정점 위치
    float4 Color    : COLOR;       // 색상
    float2 TexCoord : TEXCOORD0;   // 텍스처 좌표
    float3 Normal   : NORMAL;      // 노말
    float3 Tangent  : TANGENT;     // 탄젠트 벡터
    float3 Binormal : BINORMAL;    // 바이노멀 벡터
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