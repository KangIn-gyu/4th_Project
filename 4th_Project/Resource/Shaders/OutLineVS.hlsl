#include "Helper.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    // 기본 변환
    float4 worldPos = mul(float4(input.Position.xyz, 1.0f), worldMatrix);
    float4 viewPos = mul(worldPos, viewMatrix);
    float4 projPos = mul(viewPos, projectionMatrix);
    
    if (onOutline)
    {
        // 뷰 공간의 노말을 NDC 공간으로 변환
        float3 viewNormal = normalize(mul(mul(float4(input.Normal, 0.0f), worldMatrix), viewMatrix).xyz);
        float2 ndcNormal = normalize(mul(float4(viewNormal, 0.0f), projectionMatrix).xy);
        
        // NDC 공간에서 일정한 두께로 확장
        float OutlineWidth = 0.005f;
        projPos.xy += ndcNormal * OutlineWidth * projPos.w;
    }
    
    output.Position = projPos;
    output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    return output;
}