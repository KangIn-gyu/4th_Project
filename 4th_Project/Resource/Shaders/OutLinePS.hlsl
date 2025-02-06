#include "Helper.hlsli"

float4 OutlinePS(PixelInputType input) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f); // 아웃라인 색상
}