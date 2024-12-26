#include "Header.hlsli"

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    textureColor = TexutureColor.Sample(samLinear, input.Tex);

    return input.Color;
}