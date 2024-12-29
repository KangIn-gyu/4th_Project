#include "Header.hlsli"

float4 main(PixelInputType input) : SV_TARGET
{
    int ID = input.VertexID;
    float4 textureColor;
    textureColor = TexutureColor.Sample(samLinear, input.TexCoord);
    textureColor = textureColor + input.Color;
    return textureColor;
}