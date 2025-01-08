#include "Header.hlsli"  // 파일 기준으로 경로를 들어가야 한다

float4 main(PixelInputType input) : SV_TARGET
{
 //   int ID = input.VertexID;
    float4 textureColor;
    textureColor = TextureAlbedo.Sample(samLinear, input.TexCoord);
    textureColor = textureColor * input.Color;
    return textureColor;
}