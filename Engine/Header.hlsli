#include "TextureHeader.hlsli"
#include "SamplerHeader.hlsli" 

// 0 ~ 13 (슬롯 번호) // (4096 *) 16 byte 단위로 정렬해야 한다.
cbuffer MatrixBuffer : register(b0) // ConstantBuffer 슬롯
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightBuffer : register(b1) // ConstantBuffer 슬롯
{

};


struct VertexInputType
{
    float4 Position : POSITION;
    float4 Color :    COLOR;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float4 Color :    COLOR;
    float2 Tex :      TEXCOORD0;
};

// 일단 하나로 통일해서 보기 쉽게 처리를 함