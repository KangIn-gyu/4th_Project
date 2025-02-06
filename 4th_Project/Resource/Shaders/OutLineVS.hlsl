#include "Helper.hlsli"

PixelInputType OutlineVS(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0; // 나머지 값들은 0으로
    
    // 법선 방향으로 정점 확장
    float3 positionW = mul(input.Position, worldMatrix).xyz;
    float3 normalW = normalize(mul(float4(input.Normal, 0.0f), worldMatrix).xyz);
    positionW += normalW * 0.03f;
    
    output.Position = mul(mul(float4(positionW, 1.0f), viewMatrix), projectionMatrix);
    
    return output;
}