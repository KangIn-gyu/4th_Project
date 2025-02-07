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
        // 주변 버텍스의 노말을 고려한 스무딩된 노말 계산
        float3 smoothedNormal = input.Normal;
        
        // 탄젠트가 있는 경우에만 보간 수행
        if (length(input.Tangent) > 0.0f)
        {
            float3 tangent = normalize(input.Tangent);
            float3 bitangent = normalize(cross(input.Normal, tangent));
            
            // TBN 행렬 생성
            float3x3 TBN = float3x3(tangent, bitangent, input.Normal);
            
            // 탄젠트 공간에서 노말 보간
            float smoothingFactor = 0.5f;
            smoothedNormal = lerp(input.Normal, mul(input.Normal, TBN), smoothingFactor);
            smoothedNormal = normalize(smoothedNormal);
        }
        
        // 뷰 공간의 스무딩된 노말을 NDC 공간으로 변환
        float3 viewNormal = normalize(mul(mul(float4(smoothedNormal, 0.0f), worldMatrix), viewMatrix).xyz);
        float2 ndcNormal = normalize(mul(float4(viewNormal, 0.0f), projectionMatrix).xy);
        
        float OutlineWidth = 0.005f;
        projPos.xy += ndcNormal * OutlineWidth * projPos.w;
    }
    
    output.Position = projPos;
    output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    return output;
}