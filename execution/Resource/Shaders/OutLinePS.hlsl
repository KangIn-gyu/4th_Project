#include "Helper.hlsli"

float4 main(PixelInputType input) : SV_Target
{
    input.Color;
    return input.Color;
}